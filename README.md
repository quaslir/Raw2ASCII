# Raw2ASCII

## Overview
A low-level binary-to-text translator. Focused on deep parsing of media file structures and high-speed, buffer-optimized graphical output directly to the console.

## 📋 Table of Contents
* [Technical Features](#-technical-features)
* [Installation](#-installation)
* [Usage](#-usage)
* [Command Line Arguments](#-command-line-arguments)
* [Pipes](#-pipes)
* [Supported Media Types](#-supported-media-types)

---

## 🛠 Technical Features

* **FFmpeg Core:** High-speed decoding via `libavcodec`.
* **Audio Engine:** `miniaudio` with a dedicated ring buffer for sync.
* **Multithreaded:** Producer-consumer architecture.
* **Image and GIF rendering:** Powered by the **stb_image** library for fast and lightweight decoding.
* **TrueColor:** Full 24-bit RGB ANSI support.

## 📦 Installation

```bash
# Install dependencies (macOS)
brew install ffmpeg

# Build project
mkdir build && cd build
cmake ..
make
```

## 🚀 Usage

To run the player, simply provide the path to a media file. If no path is provided (or if `-` is used), the program automatically reads from **stdin**.

```bash
# Play a video file
./RAW2ASCII video.mp4 -w200 -h70 -f -t5

# Display a static image or GIF
./RAW2ASCII image.png -w200 -h70
```

### 🎮 Command Line Arguments

| Short Flag | Long Flag | Description | Default | Example |
| :--- | :--- | :--- | :--- | :--- |
| | `--help` | List of arguments and useful data | — | `--help` |
| `-w` | `--width=` | Target width in characters | full width | `-w120` |
| `-h` | `--height=` | Target height in characters | 80% of full height | `--height=60` |
| `-o` | `--output=` | Save standard output to provided file | `stdout` | `--output=dump.txt` |
| `-b` | `--braille` | Enable Braille rendering method | Block | `-b` |
| `-t` | `--threshold=` | Set similarity threshold between colors to increase FPS | 0 | `-t5` |
| `-f` | `--fps` | Enable real-time FPS rendering | Off | `-f` |

### 🔗 Pipes
`RAW2ASCII` also supports pipes, which is extremely useful for processing remote streams or chaining tools. For example, you can pipe an image or video directly from the web:

```bash
wget -qO- 'images-assets.nasa.gov/image/GSFC_20171208_Archive_e000671/GSFC_20171208_Archive_e000671~orig.jpg' | ./RAW2ASCII -w200 -h70
```
*This command will render the "Stormy seas in Sagittarius" picture directly from nasa.gov into your terminal.*

### 🎞️ Supported media types

The player supports a wide range of formats through a combination of **stb_image** and **FFmpeg**:

* **Images:** `.png`, `.jpg`, `.jpeg`, `.bmp`, `.gif`, `.tga`, `.pic`, `.psd`.
* **Videos:** `.mkv`, `.mp4`, `.mov`, `.avi`, `.flv`, `.webm`, and others.

---
