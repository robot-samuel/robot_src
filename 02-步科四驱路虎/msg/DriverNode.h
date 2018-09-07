// Generated by gencpp from file dgvmsg/DriverNode.msg
// DO NOT EDIT!


#ifndef DGVMSG_MESSAGE_DRIVERNODE_H
#define DGVMSG_MESSAGE_DRIVERNODE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace dgvmsg
{
template <class ContainerAllocator>
struct DriverNode_
{
  typedef DriverNode_<ContainerAllocator> Type;

  DriverNode_()
    : name()
    , add(0)
    , VRPM(0)
    , Vspeed(0.0)  {
    }
  DriverNode_(const ContainerAllocator& _alloc)
    : name(_alloc)
    , add(0)
    , VRPM(0)
    , Vspeed(0.0)  {
  (void)_alloc;
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _name_type;
  _name_type name;

   typedef int32_t _add_type;
  _add_type add;

   typedef int32_t _VRPM_type;
  _VRPM_type VRPM;

   typedef double _Vspeed_type;
  _Vspeed_type Vspeed;




  typedef boost::shared_ptr< ::dgvmsg::DriverNode_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::dgvmsg::DriverNode_<ContainerAllocator> const> ConstPtr;

}; // struct DriverNode_

typedef ::dgvmsg::DriverNode_<std::allocator<void> > DriverNode;

typedef boost::shared_ptr< ::dgvmsg::DriverNode > DriverNodePtr;
typedef boost::shared_ptr< ::dgvmsg::DriverNode const> DriverNodeConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::dgvmsg::DriverNode_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::dgvmsg::DriverNode_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace dgvmsg

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'dgvmsg': ['/home/pc-robot/workspace/ubuntudev/03-controler/02-controller_232/src/dgvmsg/msg'], 'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/indigo/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::dgvmsg::DriverNode_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::dgvmsg::DriverNode_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dgvmsg::DriverNode_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dgvmsg::DriverNode_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dgvmsg::DriverNode_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dgvmsg::DriverNode_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::dgvmsg::DriverNode_<ContainerAllocator> >
{
  static const char* value()
  {
    return "97dea7a29a79ed3e3ede5691994f3c14";
  }

  static const char* value(const ::dgvmsg::DriverNode_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x97dea7a29a79ed3eULL;
  static const uint64_t static_value2 = 0x3ede5691994f3c14ULL;
};

template<class ContainerAllocator>
struct DataType< ::dgvmsg::DriverNode_<ContainerAllocator> >
{
  static const char* value()
  {
    return "dgvmsg/DriverNode";
  }

  static const char* value(const ::dgvmsg::DriverNode_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::dgvmsg::DriverNode_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# This represents a vector in free space. \n\
# It is only meant to represent a direction. Therefore, it does not\n\
# make sense to apply a translation to it (e.g., when applying a \n\
# generic rigid transformation to a Vector3, tf2 will only apply the\n\
# rotation). If you want your data to be translatable too, use the\n\
\n\
string name\n\
int32 add\n\
int32 VRPM\n\
float64 Vspeed\n\
";
  }

  static const char* value(const ::dgvmsg::DriverNode_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::dgvmsg::DriverNode_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.name);
      stream.next(m.add);
      stream.next(m.VRPM);
      stream.next(m.Vspeed);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct DriverNode_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::dgvmsg::DriverNode_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::dgvmsg::DriverNode_<ContainerAllocator>& v)
  {
    s << indent << "name: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.name);
    s << indent << "add: ";
    Printer<int32_t>::stream(s, indent + "  ", v.add);
    s << indent << "VRPM: ";
    Printer<int32_t>::stream(s, indent + "  ", v.VRPM);
    s << indent << "Vspeed: ";
    Printer<double>::stream(s, indent + "  ", v.Vspeed);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DGVMSG_MESSAGE_DRIVERNODE_H
