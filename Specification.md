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
- [no]poly: sets wether playing multiple notes at the same time is allowed. Drastically alters the file format if enabled/disabled. A value cannot be provided for this keyword. (Later referenced as 'polynote')
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
`player (<generator-id>) { [note1] [note2] ... }`
- generator-id: a 0-based index of the generator to be used, or a new generator definition.
- note: A valid note definition

#### Notes
Notes are what players play.

##### Syntax
- If polynotes are enabled:  `<timestamp [parameters]>`
- if polynotes are disabled: `<[parameters]>`

##### Note parameters
There are multiple types of parameters that can be used.
- `<freqency> <length> [amplitude]`: Creates a note that is actually playable
  - frequency: a keyframable value, either a float or a custom 2-3 letter pitch definition based on western notation. Syntax: `<pitch>[modifier]<octave>`
    - pitch: an uppercase letter between A and G marking the pitch.
    - modifier: A pitch modifying  character resembling the ones you can find in sheets:
      - `#`: raises the pitch by 1 semitone
      - `x`: raises the pitch by 2 semitones
      - `b`: lowers the pitch by 1 semitone
    - octave: a number defining the octave of the note.
  - length: a non-keyframable value representing the note length.
  - amplitude: a keyframable value between 0 and 1 representing the amplitude of the generated wave's amplitude.
- `set([generator-id])`: sets the player's generator to the one defined. Same rules apply as when making a player.
- `loop([repetitions]) { [note1] [note2] ... }`: A looping sequence of notes.
  - repetitions: How many times the sequence must be repeated. If not defined, the sequence repeats infinitely (or until the program is killed)
  - note: a note definition. Yes loops can contain other loops, altough infinite loops cannot contain other infinite loops.
- `random([frequencies] [length] [amplitudes] [])`

