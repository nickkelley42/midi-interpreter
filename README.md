# midi-interpreter

This is a bare-bones library for interpreting a stream of MIDI data. Client code
may define callback functions, which they register to the relevant events.

# Example

```
#include "midi-callbacks.h"
#include <iostream>

// Define a callback function to handle a MIDI event
void printNoteOn(uint8_t pitch, uint8_t velocity) {
    // Do something using incoming note on message
}

int main() {
    // Create a MidiStream object, passing an istream to the constructor
    MidiStream ms {std::cin};

    // Register a callback to an event type
    ms.setCallback(Dyadic7Message::NoteOn, printNoteOn);

    // Let 'er rip
    while (ms.hasNext()) {
        ms.next();
    }
}
```

# Type Checking

MIDI events are grouped by their callback signatures. There are three categories:

* **Monadic 7-bit**: Messages that include one 7-bit value (stored as `uint8_t`)
* **Dyadic 7-bit**: Messages that include two 7-bit values (stored as `uint8_t`)
* **Monadic 14-bit**: Messages that include one 14-bit value (stored as `uint16_t`)

Callback functions are checked at compile-time to ensure that the callback
signature matches the message to which it is being registered.
