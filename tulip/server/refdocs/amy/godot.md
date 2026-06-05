# AMY in Godot

The AMY synthesizer engine works as a [GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/index.html) addon for Godot 4.3+, with support for both **native** (macOS, Linux, Windows) and **web** exports.

On native platforms, AMY runs as a C library via GDExtension and routes audio through Godot's `AudioStreamGenerator`. On web, AMY runs its own WASM module with AudioWorklet and the GDScript wrapper sends wire messages via `JavaScriptBridge`.

## Quick Start

### Option A: Download pre-built addon (easiest)

Download [`amy-godot-addon.zip`](https://github.com/shorepine/amy/releases/latest/download/amy-godot-addon.zip) and unzip it into your Godot project root so you have `your_project/addons/amy/`.

On **macOS**, you need to remove the quarantine flag from the downloaded binary:

```bash
xattr -dr com.apple.quarantine addons/amy/bin/*
```

If you don't do this, you'll see a Godot error like "Apple could not verify 'libamy.macos.template_debug.universal.dylib' is free of malware that may harm your Mac or compromise your privacy." This command tells macOS you've chosen to trust the software yourself.

### Option B: Build from source

Clone AMY and [godot-cpp](https://github.com/godotengine/godot-cpp), then run the setup script:

```bash
git clone https://github.com/shorepine/amy.git
cd amy

# Clone godot-cpp next to your project (or wherever you like)
git clone --branch godot-4.4-stable https://github.com/godotengine/godot-cpp.git ../godot-cpp

# Build the addon and install it into your Godot project
./setup_godot.sh /path/to/your/godot/project
```

The script builds the native GDExtension library and copies everything into `your_project/addons/amy/`.

If you want to point to a `godot-cpp` checkout in a different location:

```bash
GODOT_CPP_PATH=/path/to/godot-cpp ./setup_godot.sh /path/to/your/godot/project
```

### 2. Open the project in the Godot editor

Open or reimport the project so Godot registers the `Amy` class.

### 3. Use AMY in your scripts

```gdscript
var amy: Amy

func _ready():
    amy = Amy.new()
    add_child(amy)
    await get_tree().process_frame  # let AMY initialize

    # Play a 440 Hz sine wave
    amy.send({"osc": 0, "wave": Amy.SINE, "freq": 440, "vel": 1.0})

    # Play a MIDI note on a triangle wave
    amy.send({"osc": 1, "wave": Amy.TRIANGLE, "note": 60, "vel": 0.5})

    # Stop oscillator 0
    amy.send({"osc": 0, "vel": 0})

    # Use a patch (preset instrument)
    amy.send({"synth": 1, "patch": 1, "num_voices": 6, "note": 48, "vel": 0.8})

    # Or use wire protocol directly
    amy.send_raw("v3w0f880l0.5")
```

### 4. Configure AMY (optional)

Set [config properties](api.md)  on the `Amy` node **before** adding it to the tree:

```gdscript
var amy: Amy

func _ready():
    amy = Amy.new()
    amy.startup_bleep = false
    amy.default_synths = true
    add_child(amy)  # config is applied when AMY starts in _ready()
```

## Web Export

AMY works on web exports. The native GDExtension isn't used on web — instead, AMY's pre-built WASM module runs via JavaScript and the `Amy` class automatically switches to using `JavaScriptBridge`.

### Setup steps

1. **Run the install script** in the Godot editor:
   - Open `addons/amy/install.gd` in the Script Editor
   - Run it via **File > Run** (or `Ctrl/Cmd+Shift+X`)
   - This copies web audio files to the right locations

2. **Configure the web export preset**:
   - Open **Project > Export > Web**
   - Set **Custom HTML Shell** to `res://export/custom_shell.html`
   - Set **Exclude Filter** to `addons/amy/bin/*,addons/amy/src/*,addons/amy/amy_src/*,addons/amy/web/*,addons/amy/SConstruct,addons/amy/install.gd,addons/amy/amy.gdextension` (exclude native libraries and build files, but keep `amy.gd`)

3. **Export to a separate folder** (e.g. `dist/` inside your project):
   - Click **Export Project** and save the `.html` file into a new folder (e.g. `dist/YourGame.html`)
   - Godot will place all its export files there automatically

4. **Copy AMY's web audio files** into the export folder:
   ```bash
   cp -r addons/amy/web/ dist/web_audio/
   cp addons/amy/web/enable-threads.js dist/
   ```

5. **Deploy** — upload the contents of `dist/` to your web server. The folder should contain:
   ```
   YourGame.html                       # main page
   YourGame.js                         # Godot engine
   YourGame.wasm                       # Godot WASM binary
   YourGame.pck                        # packed game assets
   YourGame.audio.worklet.js           # Godot audio worklet
   YourGame.audio.position.worklet.js  # Godot audio position worklet
   enable-threads.js                   # COOP/COEP service worker for AudioWorklet support
   web_audio/                          # AMY audio engine
     amy.js
     amy.wasm
     amy.aw.js
     amy.ww.js
     godot_amy_bridge.js
     enable-threads.js
   ```

Or run locally: `python3 -m http.server` from your `dist`  folder and go to `localhost:8000`.


## How It Works

- **Native (macOS/Linux/Windows):** AMY runs as a GDExtension (`AmySynth` C++ class) compiled with `-DAMY_NO_MINIAUDIO` and `AMY_AUDIO_IS_NONE`. Audio is rendered via `amy_simple_fill_buffer()` and routed through Godot's `AudioStreamGenerator` at 44100 Hz.

- **Web:** AMY runs as its own WASM module with Web Audio API AudioWorklets. The `Amy` GDScript class detects `OS.get_name() == "Web"` and sends wire messages via `JavaScriptBridge` instead of the native extension.


## API Reference

### `amy.send(params: Dictionary)`

Send a message to AMY using named parameters. This mirrors AMY's Python API — the parameter names are the same as `amy.send()` in Python.

**Common parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `osc` | int | Oscillator number (0-63) |
| `wave` | int | Wave type (use constants like `Amy.SINE`) |
| `freq` | float | Frequency in Hz |
| `note` | int/float | MIDI note number |
| `vel` | float | Velocity / volume (0.0 = off, 1.0 = max) |
| `amp` | float | Amplitude |
| `duty` | float | Pulse width duty cycle |
| `pan` | float | Stereo panning |
| `patch` | int | Preset patch number |
| `filter_freq` | float | Filter cutoff frequency |
| `filter_type` | int | Filter type (use `Amy.FILTER_LPF` etc.) |
| `resonance` | float | Filter resonance / Q |
| `feedback` | float | FM feedback amount |
| `ratio` | float | FM frequency ratio |
| `algorithm` | int | FM algorithm number |
| `bp0` | string | Breakpoint envelope 0 (time,val pairs) |
| `bp1` | string | Breakpoint envelope 1 |
| `volume` | float | Global volume |
| `tempo` | float | Sequencer tempo in BPM |
| `chorus` | string | Chorus settings |
| `reverb` | string | Reverb settings |
| `echo` | string | Echo/delay settings |

See the full [AMY API reference](api.md) for all available parameters.

### `amy.send_raw(msg: String)`

Send a raw AMY wire-protocol message (e.g. `"v0w0f440l1"`).

### `amy.panic()`

Stop all sound immediately.

### Constants

**Wave types:** `Amy.SINE`, `Amy.PULSE`, `Amy.SAW_DOWN`, `Amy.SAW_UP`, `Amy.TRIANGLE`, `Amy.NOISE`, `Amy.KS`, `Amy.PCM`, `Amy.ALGO`, `Amy.PARTIAL`, `Amy.WAVETABLE`, `Amy.CUSTOM`, `Amy.WAVE_OFF`

**Filter types:** `Amy.FILTER_NONE`, `Amy.FILTER_LPF`, `Amy.FILTER_BPF`, `Amy.FILTER_HPF`, `Amy.FILTER_LPF24`

**Envelope types:** `Amy.ENVELOPE_NORMAL`, `Amy.ENVELOPE_LINEAR`, `Amy.ENVELOPE_DX7`, `Amy.ENVELOPE_TRUE_EXPONENTIAL`
