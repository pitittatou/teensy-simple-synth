# teensy-simple-synth

Simple polyphonic synthetizer for the teensy microcontroller, using a modular architecture.
It also includes a configurable arpeggiator.
The teensy USB type must be configured as Serial + MIDI.

To configure the synthetizer while it is running, send commands via the Serial interface.

## Configure a new voice sound
Add an if-else block in the createVoices function and choose the waveform and envelope. Increment the `VOICE_SOUND_NB` define constant.

To change the synthetizer voice sound while it is running, use:
- `voice <v>` --> change the voice sound to v, v must be a valid int between `0` and `VOICE_SOUND_NB`

## Arpeggiator related commands
- `arp on` --> enable the arpeggiator
- `arp off` --> disable the arpeggiator
- `arp mode <m>` --> change the arpeggiator mode, m can be:
    - `up` --> play notes from bottom to top
    - `down` --> play notes from top to bottom
    - `updown` --> play notes from bottom to top then top to bottom
    - `downup` --> play notes from top to bottom then bottom to top
- `arp note <d>` --> configure the note duration in seconds, d must be a valid float
- `arp inter <d>` --> configure the note interval duration in seconds, d must be a valid float
