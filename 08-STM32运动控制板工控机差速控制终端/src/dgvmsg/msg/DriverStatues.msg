# This represents a vector in free space. 
# It is only meant to represent a direction. Therefore, it does not
# make sense to apply a translation to it (e.g., when applying a 
# generic rigid transformation to a Vector3, tf2 will only apply the
# rotation). If you want your data to be translatable too, use the

int32 device_status
int32 device_add
int32 actvelocityRPM
int32 setvelocityRPM
uint16 dc_voltage
uint16 ipm_temperature
float32 actcurrent
int32 actualpos
int32 emergencyinputvalue
string errorcode

