import os
import pytest
import numpy as np
from cffi import FFI
from scipy.signal import hilbert

ffi = FFI()

# Define C structures and function signatures
ffi.cdef("""
    typedef struct {
        float depth;
        float rate;
        float sample_rate;
        float phase;
    } Tremolo;

    typedef struct {
        float master_gain;
        Tremolo tremolo;
    } AudioPipeline;

    void tremolo_init(Tremolo *t, float depth, float rate, float sample_rate);
    void audio_pipeline_init(AudioPipeline *pipeline, float sample_rate);
    void audio_pipeline_process_float(AudioPipeline *pipeline, const float *in_buf, float *out_buf, uint32_t length);
    void audio_pipeline_process_12bit(AudioPipeline *pipeline, const uint16_t *in_buf, uint16_t *out_buf, uint32_t length);
""")

@pytest.fixture
def dsp():
    lib_path = os.path.abspath("./libdsp.dll") # Use libdsp.dll on Windows
    return ffi.dlopen(lib_path)

def test_pipeline_12bit_adc_dac_passthrough(dsp):
    """
    Test 12-bit processing conversion bounds (0 to 4095) with zero tremolo depth.
    """
    pipeline = ffi.new("AudioPipeline *")
    dsp.audio_pipeline_init(pipeline, 48000.0)
    pipeline.tremolo.depth = 0.0  # Unity gain pass-through

    block_size = 256
    raw_adc_in = np.full(block_size, 3072, dtype=np.uint16)  # Mid-high DC voltage
    raw_dac_out = np.zeros(block_size, dtype=np.uint16)

    in_ptr = ffi.cast("uint16_t *", ffi.from_buffer(raw_adc_in))
    out_ptr = ffi.cast("uint16_t *", ffi.from_buffer(raw_dac_out))

    dsp.audio_pipeline_process_12bit(pipeline, in_ptr, out_ptr, block_size)

    # Output should equal input under unity gain
    np.testing.assert_array_equal(raw_dac_out, raw_adc_in)

def test_pipeline_streaming_frame_continuity(dsp):
    """
    Simulate streaming consecutive audio frames to verify LFO phase continuity across blocks.
    """
    sample_rate = 48000.0
    block_size = 256
    num_blocks = 10

    pipeline = ffi.new("AudioPipeline *")
    dsp.audio_pipeline_init(pipeline, sample_rate)

    # Generate continuous 440 Hz Sine wave input
    total_samples = block_size * num_blocks
    time_axis = np.arange(total_samples) / sample_rate
    full_input_stream = np.sin(2 * np.pi * 440.0 * time_axis).astype(np.float32)
    
    full_output_stream = np.zeros(total_samples, dtype=np.float32)

    # Stream through pipeline block-by-block (mimicking DMA interrupts)
    for block_idx in range(num_blocks):
        start = block_idx * block_size
        end = start + block_size

        in_block = full_input_stream[start:end]
        out_block = np.zeros(block_size, dtype=np.float32)

        in_ptr = ffi.cast("float *", ffi.from_buffer(in_block))
        out_ptr = ffi.cast("float *", ffi.from_buffer(out_block))

        dsp.audio_pipeline_process_float(pipeline, in_ptr, out_ptr, block_size)

        full_output_stream[start:end] = out_block

    # Assertions
    # 1. Output amplitude envelope should modulate due to tremolo
    assert np.max(full_output_stream) <= 1.0
    assert np.min(full_output_stream) >= -1.0

    # 2. Check that state retained phase (phase > 0 after streaming)
    assert pipeline.tremolo.phase > 0.0

def test_soft_clipping_protection(dsp):
    """
    Ensure excessive gain saturates properly at +/- 1.0 float bounds.
    """
    pipeline = ffi.new("AudioPipeline *")
    dsp.audio_pipeline_init(pipeline, 48000.0)
    pipeline.master_gain = 5.0  # Overdrive pipeline

    in_block = np.array([0.5, 0.8, -0.9, 1.2], dtype=np.float32)
    out_block = np.zeros(4, dtype=np.float32)

    in_ptr = ffi.cast("float *", ffi.from_buffer(in_block))
    out_ptr = ffi.cast("float *", ffi.from_buffer(out_block))

    dsp.audio_pipeline_process_float(pipeline, in_ptr, out_ptr, 4)

    # Ensure hard-coded clipping holds
    assert np.all(out_block <= 1.0)
    assert np.all(out_block >= -1.0)

def test_tremolo_depth_and_rate_modulation(dsp):
    sample_rate = 48000.0
    block_size = 256
    
    target_rate = 5.0
    target_depth = 0.8
    duration = 2.0  # Increase duration slightly for better FFT frequency resolution
    total_samples = int(sample_rate * duration)
    num_blocks = total_samples // block_size

    pipeline = ffi.new("AudioPipeline *")
    dsp.audio_pipeline_init(pipeline, sample_rate)
    dsp.tremolo_init(ffi.addressof(pipeline.tremolo), target_depth, target_rate, sample_rate)

    # 1 kHz carrier
    t = np.arange(total_samples) / sample_rate
    carrier = np.sin(2 * np.pi * 1000.0 * t).astype(np.float32)
    output = np.zeros(total_samples, dtype=np.float32)

    for i in range(num_blocks):
        start, end = i * block_size, (i + 1) * block_size
        in_ptr = ffi.cast("float *", ffi.from_buffer(carrier[start:end]))
        out_ptr = ffi.cast("float *", ffi.from_buffer(output[start:end]))
        dsp.audio_pipeline_process_float(pipeline, in_ptr, out_ptr, block_size)

    # Extract envelope
    analytic_signal = hilbert(output)
    amplitude_envelope = np.abs(analytic_signal)

    trim = int(total_samples * 0.10)
    steady_envelope = amplitude_envelope[trim:-trim]

    # Verify Depth
    env_max = np.max(steady_envelope)
    env_min = np.min(steady_envelope)
    np.testing.assert_allclose(env_max, 1.0, atol=0.05)
    np.testing.assert_allclose(env_min, 1.0 - target_depth, atol=0.05)

    # Verify Frequency via Spectral Peak (FFT)
    # Remove DC component to isolate modulation signal
    env_ac = steady_envelope - np.mean(steady_envelope)
    fft_vals = np.abs(np.fft.rfft(env_ac))
    fft_freqs = np.fft.rfftfreq(len(env_ac), d=1.0/sample_rate)

    measured_lfo_rate = fft_freqs[np.argmax(fft_vals)]
    
    np.testing.assert_allclose(measured_lfo_rate, target_rate, atol=0.2)