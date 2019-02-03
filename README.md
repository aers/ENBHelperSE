# MiscFixesSSE

## Build Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [CommonLibSSE](https://github.com/SniffleMan/CommonLibSSE)

## End User Dependencies
* [SKSE64](https://skse.silverlock.org/)

## Bugs Fixed
* When using the `IsRunning` function available to perk fragments on anything that isn't an actor, the game will crash.
* If the player learns a spell from a book, and then loads an `.esp` that removes that book's spell, the game will crash when the book is displayed in the inventory.
* A bug where memory is used after being freed
