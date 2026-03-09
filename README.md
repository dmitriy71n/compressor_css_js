**Compressor**


A multifunctional tool for **minification, formatting, merging, and conversion of web resources** (HTML, CSS, JS, SASS).

## Features

* Minification of **HTML, CSS, and JS** files into a single-line format (e.g., `file.min.css`).
* Formatting minified files back into a readable form (e.g., `file.fmt.css`).
* Conversion of **SASS/SCSS** files to **CSS**.
* Processing of combined **HTML + CSS + JS** files.
* Merging multiple **CSS files into a single file** in alphabetical order.
* **Multilingual interface** (Russian, English).

The compressor uses **Qt6 libraries**.
It is based on the **dart-sass** library.

Updating the **dart-sass** library is done by replacing the library in the `dart-sass` folder on Linux, or in the `sass` folder on Windows.
[https://github.com/sass/dart-sass/releases](https://github.com/sass/dart-sass/releases)

The default included version is **dart-sass-1.77.8**.
Tested with **dart-sass-1.97.1**.

---

# Installation on Windows 10 / 11

* Open the **Compressor** folder
* Run `compressor.exe`

---

# Installation on Linux

### Debian / Ubuntu / Mint

```
sudo apt install libqt6widgets6
```

### Arch Linux

```
sudo pacman -S qt6-base
```

### Fedora

```
sudo dnf install qt6-qtbase
```

Extract the archive `compressor_2.0_linux.zip`

```
cp compressor/compressor.desktop ~/.local/share/applications/
sudo cp -r compressor/ /opt
sudo chown -R root:root /opt/compressor
sudo chmod -R 755 /opt/compressor
```

Run:

```
/opt/compressor/bin/./compressor
```

If you encounter the error:

```
error while loading shared libraries: libQt6Widgets.so.6: 
cannot open shared object file: No such file or directory
```

Check whether the Qt6 libraries **libqt6widgets6**, **qt6-base**, or **qt6-qtbase** are installed.

If the **Compressor** shortcut does not appear in the application menu, restart your graphical session.
