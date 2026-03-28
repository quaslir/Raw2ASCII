# Raw2ASCII
![Build Status](https://github.com/quaslir/Raw2ASCII/actions/workflows/ci.yml/badge.svg)
## Overview
A low-level binary-to-text translator. Focused on deep parsing of media file structures and high-speed, buffer-optimized graphical output directly to the console.

## 🎬 Showcase

<p align="center">
  <img width="498" src="https://github.com/user-attachments/assets/557811d2-378b-4526-a1ad-c9fc38dbd9e3" alt="Raw2ASCII Screenshot" />
</p>

<p align="center">
  <video src="https://github.com/user-attachments/assets/49e06c54-7f3a-4716-be86-4acea979c99d" width="100%" autoplay loop muted playsinline alt="Nasa video rendered in terminal"></video>
</p>



## 📋 Table of Contents
- [Raw2ASCII](#raw2ascii)
  - [Overview](#overview)
  - [🎬 Showcase](#-showcase)
  - [📋 Table of Contents](#-table-of-contents)
  - [🛠 Technical Features](#-technical-features)
    - [🛠️ Prerequisites](#️-prerequisites)
  - [📦 Installation](#-installation)
  - [🚀 Usage](#-usage)
    - [🎮 Command Line Arguments](#-command-line-arguments)
    - [🔗 Pipes](#-pipes)
    - [🎞️ Supported media types](#️-supported-media-types)
    - [⚡ Performance Tips](#-performance-tips)
  - [📄 License](#-license)

---

## 🛠 Technical Features

* **FFmpeg Core:** High-speed decoding via `libavcodec`.
* **Audio Engine:** `miniaudio` with a dedicated ring buffer for sync.
* **Multithreaded:** Producer-consumer architecture.
* **Image and GIF rendering:** Powered by the **stb_image** library for fast and lightweight decoding.
* **TrueColor:** Full 24-bit RGB ANSI support.

### 🛠️ Prerequisites

Before building, ensure you have the following installed:

* **FFmpeg 8.1+** (specifically `libavcodec`, `libavformat`, `libswscale`)
* **CMake 4.2+**
* **C++20 Compiler**
> [!IMPORTANT]
> **Terminal Support:** To see the video in high quality, you **MUST** use a terminal that supports **TrueColor (24-bit RGB)**. 
> Recommended: **Alacritty**, **Kitty**, **iTerm2**, or **Windows Terminal**. 
> *Standard macOS Terminal.app does NOT support TrueColor and will display limited colors.*

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

### ⚡ Performance Tips

To get the most out of **Raw2ASCII**, especially for high-resolution video, consider the following:

* **Terminal Choice:** Use a GPU-accelerated terminal like **Alacritty**, **Kitty**, or **iTerm2**. Standard terminals (like the default macOS Terminal) often struggle with high-frequency ANSI color updates.
* **Threshold Tuning (`-t`):** Increase the similarity threshold (e.g., `-t10`) to skip rendering pixels that are nearly identical to the previous frame. This significantly reduces the CPU load and bandwidth used by the terminal.
* **Font Size:** Reducing your terminal font size allows for higher `-w` (width) and `-h` (height) values, resulting in much higher "resolution" ASCII art.

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
