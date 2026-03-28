# Hangman Game (SFML, C++)

## Overview
This project is a desktop Hangman game written in C++ using SFML for graphics, audio, text rendering, input events, and window handling.

Main source file:
- Hangman-trial.cpp

The game supports:
- Random word and hint generation
- Keyboard letter guessing
- Correct/wrong guess handling
- Progressive hangman drawing on wrong guesses
- Win/loss detection
- Score display (wins/losses)
- Play, Rules, and Close screens
- Sound effects and intro sound

## How The System Recognizes A Letter And Places It In The Correct Position
This is the core logic used when the player types a letter.

1. The game reads text input events from SFML.
2. It accepts only valid alphabetic ASCII characters.
3. The typed character is converted to uppercase.
4. A letter index is computed to track repeated guesses:

   index = uppercaseLetter - 'A'

5. If the letter was already used, it is ignored.
6. If not used, the game checks the typed letter against each position of the hidden answer string.
7. For each matching position, the underscore in guessedString is replaced with the actual letter.
8. The updated guessedString is rendered on screen.

### Why this works
- The answer is stored in correctString.
- Player progress is stored in guessedString.
- The guessWord(letter) function scans the full word and updates all matching positions, not just one.
- This allows repeated letters in a word to be revealed together.

## Code Structure

### 1) LegacyRect
Purpose:
- A simple rectangle hit-test helper for button clicks.

Used for:
- Play, Rules, and Close button click detection.

### 2) Game class
Purpose:
- Base game state and scoring fields.

Contains:
- tries (protected)
- static max_tries
- static wins
- static losses
- isLost() check

Loss rule used:
- isLost() returns true when tries > max_tries.
- max_tries is set to 6.

### 3) Hangman class (inherits Game)
Purpose:
- Stores and manages word-guessing state.

Contains:
- correctString: full answer word
- guessedString: underscores + revealed letters
- hint: current hint text

Main methods:
- setCorrectString(word): sets answer
- setGuessedString(): initializes guessedString to "_____"
- guessWord(letter): reveals letter at all matching indexes
- isWon(): true when guessedString == correctString
- increaseTries(): increments wrong attempts

## Full Functionality Report

### Game initialization
- Seeds random generator with current time.
- Defines 5 words and 5 hints.
- Picks a random index and loads a word + hint into the Hangman object.
- Creates the render window (900x900).
- Loads image, fonts, and sounds.
- Creates all text objects, shapes, and default positions.

### Asset loading
Assets are loaded from these folders:
- images/
- Fonts/
- Audio/

Current referenced files:
- images/squared-paper.jpg
- Fonts/GreatVibes-Regular.otf
- Fonts/Montserrat-Light.otf
- Audio/Wrong-answer-sound-effect.wav
- Audio/Vip.wav
- Audio/wrong.wav
- Audio/correct.wav

### Menu behavior
- Play button:
  - Resets game state
  - Picks a new random word and hint
  - Clears used letters
  - Resets tries to 0
  - Hides menu UI and starts play mode
- Rules button:
  - Shows rules text screen
  - Repositions/hides gameplay UI
- Close button:
  - Plays sound and closes window

### Input handling behavior
- Receives text input events.
- Accepts only letters.
- Converts to uppercase for matching uppercase words.
- Prevents duplicate letter processing with choice[] array.

### Correct guess behavior
- Plays correct sound.
- Updates guessed text display.
- Checks win condition:
  - Shows win message
  - Updates wins counter
  - Displays scoreboard
  - Enables Play Again state

### Wrong guess behavior
- Plays wrong sound.
- Increases tries.
- Draws hangman in stages based on tries count:
  - Stand parts
  - Head
  - Body
  - Arms
  - Legs
- Checks loss condition:
  - Shows loss message with correct answer
  - Updates losses counter
  - Displays scoreboard
  - Enables Play Again state

### Rendering loop
Each frame:
1. Poll events
2. Process close/mouse/keyboard events
3. Clear window
4. Draw background sprite
5. Draw shapes and text
6. Display frame

## Libraries Used (Detailed: What, When, Why)

### iostream
Why used:
- Console output for debugging and status logs.

Used when:
- Button presses
- Typed character debugging
- Win/loss value printing

### string
Why used:
- Word storage, hint storage, message creation, and string replacement.

Used when:
- Managing correctString and guessedString
- Building UI text strings
- Updating guessed word state

### SFML/Graphics.hpp
Why used:
- All graphics and window APIs.

Used for:
- sf::RenderWindow (window creation and display)
- sf::Texture and sf::Sprite (background)
- sf::Font and sf::Text (title, menu, hints, score)
- sf::CircleShape and sf::RectangleShape (hangman body/stand)
- sf::Event system (mouse and text input)

### SFML/Audio.hpp
Why used:
- Sound effects and intro playback.

Used for:
- sf::SoundBuffer (loading sound data)
- sf::Sound (play, pause)

### stdlib.h
Why used:
- rand(), srand(), EXIT_FAILURE.

Used when:
- Random word selection
- Program exit on asset load failure

### ctime and time.h
Why used:
- time(0) for random seed.

Note:
- Both headers are included, but one is generally enough.

### cctype
Why used:
- Character validation and case conversion.

Used for:
- isalpha(...) to allow letters only
- toupper(...) to normalize typed input

## Important Implementation Notes
- Words are currently uppercase, and input is normalized to uppercase, so matching remains consistent.
- Font load results are not checked in the current code; if a font is missing, text may not render properly.
- Hint display uses fixed substring splits, which assumes hints are long enough.

## How To Run
1. Install SFML compatible with your compiler.
2. Build Hangman-trial.cpp.
3. Ensure Audio, Fonts, and images folders stay next to the executable.
4. Run the executable.

## Controls
- Mouse:
  - Click Play, Rules, Close
- Keyboard:
  - Type letter keys A-Z to guess

## Summary
The project is a complete GUI Hangman implementation using object-oriented C++ plus SFML. The typed-letter placement works by scanning every character index in the answer word and replacing matching underscore positions in guessedString, then re-rendering the updated text on screen.
