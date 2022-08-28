
# libqtfaststart++

Linkable library for implementing the conversion of Quicktime media files into streamable Fast Start format.
Ported over from [QtFastStart_Pipes](https://github.com/SkibbleBip/QtFastStart_Pipes), which was originally forked from [qtfaststart-java](https://github.com/ypresto/qtfaststart-java), which is a java re-implentation of  qt-faststart.c from [FFmpeg](https://github.com/FFmpeg/FFmpeg). So it's basically a port of a port of a port of a port.

Purpose of this application is to implement a library that can be imported into projects so they don't need to rely on the entire FFMPEG package for the one component present in qt-faststart.

## Compiling
Simply `cd src` and run `make` to generate both a shared and static library for GNU-Linux. Output files are in the `build` directory.

## Usage
Link the compiled library into your project and include QtFastStartCPP.hpp in your project.
Obtain the data of the media file to be processed and pass to the `QtFastStartSTD::QtFastStart` as such:

```
QtFastStartSTD::QtFastStart qtfs = QtFastStartSTD::QtFastStart(inputFileArray, inputFileArraySize);
QtFastStartSTD::ArtificialFileStream result = qtfs.fastStart();
```

The `QtFastStartSTD::ArtificialFileStream` will contain the byte array of the output file and the length of the output array.
Example usage is found in the `test` directory.

## License
Copyright (C) 2022 SkibbleBip

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
