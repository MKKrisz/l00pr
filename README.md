# l00pr
This is `l00pr`, a basic music player library / synth / audio tracker.

## Usage
See: [Specification.md](Specification.md)
Currently the project is too small for binary distribution I believe, so if you want to use it, refer to the building section just below.

## Building
The project uses CMake as its build system. The only dependent library for building is SDL2. You may need to tweak the root CMakeLists.txt and some includes depending on what system you try to build on.
At one point I successfully built it for Windows, Android (using termux) and ArchLinux, but your mileage may vary

## Contributing
~For now, this is my university project, so I won't be accepting any pull-requests or commits. That said, feel free to contribute by creating issues and feature suggestions!~

For a while this information has been outdated, I am open to contribution, feature requests, bugreports, etc... My current goal is making it as modular as possible while sacrificing as little performance as possible. There are most likely a bunch of optimizations that can still be done, so feel free to implement some!
