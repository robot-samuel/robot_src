# This represents a vector in free space. 
# It is only meant to represent a direction. Therefore, it does not
# make sense to apply a translation to it (e.g., when applying a 
# generic rigid transformation to a Vector3, tf2 will only apply the
# rotation). If you want your data to be translatable too, use the

int32 device_status
int32[] encounter
uint8[] Motor_Status
int16[] errorcode

int32[] Obt_dis
int32[] Obt_en_Status

int32[] input_IO_Status
int32[] out_IO_Status

int32 fcrash_io
int32 bcrash_io

int32 fObt_en_Status
int32 bObt_en_Status

int32 LMotor_com_Status
int32 RMotor_com_Status

int32[] Obt_com_Status








