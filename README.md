# Raw2ASCII

## Overview
A low-level binary-to-text translator. Focused on deep parsing of media file structures and high-speed, buffer-optimized graphical output directly to the console.


## Command Line Arguments
`raw2ascii` supports both short and long-form flags for flexible media rendering.

### Essential Arguments
```bash
./raw2ascii <PATH_TO_MEDIA>

```

### Optional Arguments

| Short Flag | Long Flag | Description | Example |
| :--- | :--- | :--- | :--- |
| `-w` | `--width=` | Target width in characters | `-w120` |
| `-h` | `--height=` | Target height in characters | `--height=60` |
| `-o` | `--output=` | Change standard output to provided file | `--output=<PATH_TO_FILE>` |
