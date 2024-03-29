#ifndef MIDIMSG_H
#define MIDIMSG_H

#ifndef UBYTE
#define UBYTE unsigned char
#endif

#ifndef WORD
#define WORD signed short
#endif

#ifndef UWORD
#define UWORD unsigned short
#endif


typedef struct {
    UBYTE channel;
    UBYTE note;
    UBYTE velocity;
} MIDIMSG_NOTE_ON;

typedef struct {
    UBYTE channel;
    UBYTE note;
    UBYTE velocity;
} MIDIMSG_NOTE_OFF;

typedef struct {
    UBYTE channel;
    UBYTE note;
    UBYTE value;
} MIDIMSG_POLY_PRESSURE;

typedef struct {
    UBYTE channel;
    UBYTE control;
    UBYTE value;
} MIDIMSG_CONTROL_CHANGE;

typedef struct {
    UBYTE channel;
    UBYTE program;
} MIDIMSG_PROGRAM_CHANGE;

typedef struct {
    UBYTE channel;
    UBYTE value;
} MIDIMSG_CHANNEL_PRESSURE;

typedef struct {
    UBYTE channel;
    WORD value;
} MIDIMSG_PITCH_BEND;

/* System common messages */
typedef struct {
    short length;
    UBYTE *data;
} MIDIMSG_SYSEX;

typedef struct {
    UBYTE type;
    UBYTE value;
} MIDIMSG_MTC_QUARTER_FRAME;

#define MIDIMSG_MESSAGE_ABORTED 1
#define MIDIMSG_UNEXPECTED_DATA 2
#define MIDIMSG_SYSEX_TOO_LARGE 3

typedef struct {
    void (*error)(short number);
    /* Channel messages */
    void (*note_off)(MIDIMSG_NOTE_OFF*);
    void (*note_on)(MIDIMSG_NOTE_ON*);
    void (*poly_pressure)(MIDIMSG_POLY_PRESSURE*);
    void (*control_change)(MIDIMSG_CONTROL_CHANGE*);
    void (*program_change)(MIDIMSG_PROGRAM_CHANGE*);
    void (*channel_pressure)(MIDIMSG_CHANNEL_PRESSURE*);
    void (*pitch_bend)(MIDIMSG_PITCH_BEND*);

    /* System real-time messages */
    void (*clock)(void);
    void (*song_start)(void);
    void (*song_continue)(void);
    void (*song_stop)(void);
    void (*active_sensing)(void);
    void (*reset)(void);

    /* System common messages */
    void (*system_exclusive)(MIDIMSG_SYSEX *);
    void (*mtc_quarter_frame)(MIDIMSG_MTC_QUARTER_FRAME*);
    void (*song_position)(UWORD);
    void (*song_select)(UBYTE);
    void (*tune_request)(void);
} MIDIMSG_CALLBACKS;

extern MIDIMSG_CALLBACKS midimsg_callbacks;

void midimsg_init(UBYTE *sysex_buffer, short sysex_buffer_size);
void midimsg_exit(void);
void midimsg_process(UBYTE byte);

#endif
