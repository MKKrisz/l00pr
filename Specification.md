# l00pr

## Specification

The project is an overcomplicated function generator / a very simple music player

### Dependencies
- `SDL`: for audio playing

### Usage
`l00pr [arguments] <file>`

#### Arguments
The arguments may be subject to change. Listed arguments are always available but their names may differ.
- `-h` / `--help`: Prints some info and the available arguments
- `-o <file>` / `--output <file>`: If specified, instead of playing the audio, the program puts the output into `file`. The format is always .wav, regardless of the extension

### File format
The file is case-insensitive. The file consists of segments. One segment can span multiple lines. Each segment consists of a keyword and its corresponding arguments.
Supported keywords:
- `set`
- `generator(s)`
- `player`
This list may grow or change, ex. defining loops outside of notes (will be explained later)

#### Set
A `set` statement sets file-wise arguments.

##### Syntax
`set <keyword>` or `set <keyword>: <value>`

##### Keywords
- bpm: sets the beats-per-minute value. default: 60
- [no]poly: sets wether playing multiple notes at the same time is allowed. Drastically alters the file format if enabled/disabled. A value cannot be provided for this keyword.
- separator: A character that separates some specific arguments. Cannot be alfanumeric. Specific characters that are used within the program also cannot be used. No character is defined by default, and where a separator is not specified, this can be used.   
> Example: if the separator character is set to ',' by `set separator: ,` a sine generator definition (`generator sine(1 1 1)`) becomes `generator sine(1, 1, 1)`
This list mmay be subject to change.

#### Generator
A `generator` statement defines one or multiple generators for use by the note players. (explained later) Multiple statements don't override eachother, instead they add to the already existing list.

##### Syntax
`generator [type]([type arguments])` or `generator[s] { [type 1]([type arguments]) [type 2]([type arguments 2]) ... }`
Note: In both cases, if no type argument is provided, the parenthesis is not required.
Note 2: The second definition can span multiple lines

##### Generator types and their arguments
Most generators have no required parameters and a few optional arguments. Most of the parameters are keyframe-able
- sine: `[frequency] [amplitude] [phase offset]`
- square: `[frequency] [amplitude] [phase offset] [duty cycle]`
- triangle: `[frequency] [amplitude] [phase offset] [peak]`
- register: `[frequency] [amplitude] [phase offset]` This generator is special, by itself it does nothing. You must define multiple generators within its definition in a similar way to how you define multiple generators at once. Complete syntax: `register([arguments]) { [generator 1] [generator 2] ... }
This list may be subject to change.

#### Player
A `player` statement defines a generator and a list of notes for that generator to play.

##### Syntax
`player (\<generator-id\>) { \[note1\] \[note2\] }`

