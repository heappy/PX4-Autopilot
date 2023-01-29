# UWB Crazyflie PX4 board based on DWM1004c

uint64 timestamp             # time since system start (microseconds)
uint32 time_uwb_ms           # Time of UWB device in ms
uint32 counter               # Number of ranges since last start of Ranging
uint32 time_offset           # Time between ranging rounds in ms
uint16 status                # status feedback #

uint16[12] anchor_distance   # distance in cm to each UWB Anchor
bool[12] nlos                # vlos yes/no
float32[3] position          # position of the landing point in relation to the Drone (x,y,z in Meters NED)

