# Jucify

Do you have an old codebase where you never used `using namespace juce` instead of `juce::`? Too lazy to fix it? Jucify to the rescue!

## Usage:

`Usage: jucify --recursive --exclude=a,b,c <path-to-cpp-file> [more-files...]`

recursive: recursively search into directories

exclude: exclude any paths that include any of the paths

file: file or directory

## Notes:

It's not guaranteed to find every juce symbol, but just the most common ones. It also may break a few things. I tested this on the Tracktion Waveform repo and it made 20,000 changes and broke around 50 lines of code. Still a lot easier than doing it manually.

Things to watch out for: user classes with the same name as juce classes, enum entries with the same name as juce classes.

Pull requests accepted.