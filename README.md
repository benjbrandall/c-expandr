# c-expandr

## Installation and usage

1. Change `path` (top of expand.c) to your text snippet directory
2. Run install.sh, and then `./expandr`
3. Type the snippet's shortcut and then press 'New' to add its contents
4. Next time you run `./expandr`, type the snippet and hit enter to copy the snippet text to your clipboard

## Using c-expandr with a shortcut

c-expandr is designed to be triggered from a shell script that kills the last instance of expandr and starts a new one.
To use it this way, edit `run.sh` to include your own expandr installation directory, and run expandr with `run.sh`.

## TODO

* Reset the value of `full_path` so that a snippet shortcut with no matches doesn't cause problems
* Make it work properly in tiling WMs like i3


