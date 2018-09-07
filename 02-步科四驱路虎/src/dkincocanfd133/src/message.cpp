
#include "message.hpp"

#include <boost/format.hpp>

typedef      short                  int16; 
typedef      short                  INT16; 
typedef    unsigned short int       uint16; 
typedef    unsigned short int       UINT16; 

typedef    int                      int32; 
typedef    int                      INT32; 
typedef    unsigned int             uint32; 
typedef    unsigned int             UINT32;

message::message()
{
    m_cfram_info =0;
    m_Ifram_ID = 0;
    m_param1=0;
    m_param2=0;
    m_param3=0;
    m_param4=0;
    
    for (std::size_t  i = 0; i < 8; i++)  
    {  
        //m_Afram_data[i] = _char[i];
        m_Afram_data[i]=0;
    } 
    for (std::size_t  i = 0; i < 13; i++)  
    {  
        //m_Afram_data[i] = _char[i];
        data[i]=0;
    } 

}


message::message(char * buf,int len)
{
    // std::cout << _int << std::endl;  
     std::size_t size = (len>=fram_length)?fram_length:len;
    for (std::size_t  i = 0; i < size; i++)  
    {  
        //m_Afram_data[i] = _char[i];
        data[i]=buf[i];
    } 
    
    m_cfram_info =  data[0];
    char * s1 = (char *)&m_Ifram_ID;
    char * s2 = (data.c_array()+1);
    char temp[4];
    /*
    //小端
     * s1 = * s2 ;s2++,s1++;
     * s1 = * s2 ;s2++,s1++;
     * s1 = * s2 ;s2++,s1++;
     * s1 = * s2 ;s2++,s1++;
    */
     temp[0] = * s2 ;s2++;
     temp[1] = * s2 ;s2++;
     temp[2] = * s2 ;s2++;
     temp[3] = * s2 ;s2++;
     
     * s1 = temp[3] ;s1++;
     * s1 = temp[2] ;s1++;
     * s1 = temp[1] ;s1++;
     * s1 = temp[0] ;s1++;
     
     for(int index=0 ; index < 8; index++){
            m_Afram_data[index]= *(data.c_array()+5+index);
        }

     char *p = m_Afram_data.c_array();
     char *ppt=(char *)&m_param1;

       *(ppt+1) = * p ;p++;
       *(ppt) = * p ;p++;

      ppt=(char *)&m_param2;

      *(ppt+1) = * p ;p++;
      *(ppt) = * p ;p++;

    ppt=(char *)&m_param3;
    *(ppt+1) = * p ;p++;
    *(ppt) = * p ;p++;

    ppt=(char *)&m_param4;
    *(ppt+1) = * p ;p++;
    *(ppt) = * p ;p++;

}


message::message(std::string &str)
{
    // std::cout << _int << std::endl;  
    std::size_t len = str.size();
    
     std::size_t size = (len>=fram_length)?fram_length:len;
    for (std::size_t  i = 0; i < size; i++)  
    {  
        //m_Afram_data[i] = _char[i];
        data[i]=str[i];
    } 
    
    m_cfram_info =  data[0];
    char * s1 = (char *)&m_Ifram_ID;
    //char * s2 = (str.begin()+1);
    int count=1;
    char temp[4];
    /*
    //小端
     * s1 = * s2 ;s2++,s1++;
     * s1 = * s2 ;s2++,s1++;
     * s1 = * s2 ;s2++,s1++;
     * s1 = * s2 ;s2++,s1++;
    */
     temp[0] = str[count++] ;
     temp[1] = str[count++] ;
     temp[2] = str[count++] ;
     temp[3] = str[count++] ;
     
     * s1 = temp[3] ;s1++;
     * s1 = temp[2] ;s1++;
     * s1 = temp[1] ;s1++;
     * s1 = temp[0] ;s1++;
     
     for(int index=0 ; index < 8; index++){
           // m_Afram_data[index]= *(str.begin()+5+index);
           m_Afram_data[index]= str[5+index];
        }

     
      char *p = m_Afram_data.c_array();
      char *ppt=(char *)&m_param1;
     
        *(ppt+1) = * p ;p++;
        *(ppt) = * p ;p++;
     
       ppt=(char *)&m_param2;
     
       *(ppt+1) = * p ;p++;
       *(ppt) = * p ;p++;
     
     ppt=(char *)&m_param3;
     *(ppt+1) = * p ;p++;
     *(ppt) = * p ;p++;
     
     ppt=(char *)&m_param4;
     *(ppt+1) = * p ;p++;
     *(ppt) = * p ;p++;


}

message::~message()
{

}


void message::set_fram_data(char * _char)  
{  
    // std::cout << _int << std::endl;  
    for (int i = 0; i < data_length; i++)  
    {  
        m_Afram_data[i] = _char[i];
        data[5+i]=m_Afram_data[i];
    } 
}

void message::set_fram_info(char info){
    
    m_cfram_info = info;
   // int couter=0;
   // char * p = NULL;
    data[0] = m_cfram_info;
    
}
void message::set_fram_ID(UINT32 id){

    m_Ifram_ID = id;
    
    int couter=0;
   char * p = NULL;
   char tem[4] ;
   
    p = (char *)(&m_Ifram_ID);
    //大端
    tem[couter++] =   *(p);p++;
    tem[couter++] =   *(p);p++;
    tem[couter++] =   *(p);p++;
    tem[couter++] =   *(p);p++;
    
    couter=1;
    data[couter++] = tem[3];
    data[couter++] = tem[2];
    data[couter++] = tem[1];
    data[couter++] = tem[0];

    /*
    //小端
     couter=1;
    data[couter++] = tem[0];
    data[couter++] = tem[1];
    data[couter++] = tem[2];
    data[couter++] = tem[3];
    */
    
}

char message::get_fram_info(){
    return(this->m_cfram_info);
}

UINT32 message::get_fram_ID(){
      return(this->m_Ifram_ID);
}
char * message::get_fram_data(){
      return(this->m_Afram_data.c_array());
}
unsigned char message::Get_sizetype(){	
	return (unsigned char)m_Afram_data[0];
	}	

std::string message::to_string(){
    /*
    int couter=0;
    char * p = NULL;
    data[couter++] = m_cfram_info;

    p = (char *)(&m_Ifram_ID);
    
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    
    p = m_Afram_data.c_array();
    
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    data[couter++] =  *(p);p++;
    */
    
    std::string res(data.begin(),data.end());
    return res;
}

void message::print_dec(){

 std::cout<< "print_dec:";
 for(std::size_t index=0;index<fram_length;index++){
                  int x=(unsigned  char)data[index];
                   std::cout<<boost::format("%d ")%x;
              }
std::cout<<"\n";


}
 void message::print_hex(){

  std::cout<< "print_hex:";
  for(std::size_t index=0;index<fram_length;index++){
                   int x=(unsigned  char)data[index];
                    std::cout<<boost::format("%02X ")%x;
               }
 std::cout<<"\n";

 }
 
 void message::set_data_m_param1(UINT16 para){
        this->m_param1 =para;

        //int couter=0;
        char * p = NULL;  
        p = (char *)(& this->m_param1);
        
        set_data_m_byte(*p,1);
        p++;
        //大端格式
        set_data_m_byte(*p,0);

        

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */


        
    }
 void message::set_data_m_param2(UINT16 para){
         this->m_param2 =para;

        //int couter=0;
        char * p = NULL;  
        p = (char *)(& this->m_param2);
        
        //大端格式
        set_data_m_byte(*p,3);
        p++;
        set_data_m_byte(*p,2);

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */


    }
 void message::set_data_m_param3(UINT16 para){
         this->m_param3 =para;

        //int couter=0;
        char * p = NULL;  
        p = (char *)(& this->m_param3);
        
        //大端格式
        set_data_m_byte(*p,4);
        p++;
        set_data_m_byte(*p,5);

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */


    }
 void message::set_data_m_param4(UINT16 para){
         this->m_param4 =para;

        //int couter=0;
        char * p = NULL;  
        p = (char *)(& this->m_param4);
        
        //大端格式
        set_data_m_byte(*p,7);
        p++;
        set_data_m_byte(*p,6);

        //小端格式
        /*
         set_data_m_byte(*p,6);
        p++;
        set_data_m_byte(*p,7); 
        */

}
 
void message::set_data_m_param34(INT32 data){

    
 
    
    char * p = (char *)&data;
    char *pd =m_Afram_data.c_array();
    pd+=4;
    set_data_m_byte(*p,4);p++;
    set_data_m_byte(*p,5);p++;
    set_data_m_byte(*p,6);p++;
    set_data_m_byte(*p,7);p++;

    }

 void message::set_data_m_byte(char para,int pos){

        this->m_Afram_data[pos]=para;
        this->data[5+pos]=para;
        //int x=(unsigned  char)this->m_Afram_data[pos];
       //std::cout<<boost::format("[%d]:%02X")%pos%x << std::endl;      
    }
  unsigned char message::get_data_m_byte(int pos){

	unsigned char data = this->m_Afram_data[pos];
	return data;
        //int x=(unsigned  char)this->m_Afram_data[pos];
       //std::cout<<boost::format("[%d]:%02X")%pos%x << std::endl;      
    }

 
 UINT16 message::get_data_m_param1(){

     m_param1 =  this->data[5]*256+this->data[5+1];
     return this->m_param1;
    }
 
 UINT16 message::get_data_m_param2(){
    m_param2 =  this->data[5+2]*256+this->data[5+3];
    return this->m_param2;
    }
 
UINT16 message::get_data_m_param3(){
    m_param3 =  this->data[5+5]*256+this->data[5+4];
     return this->m_param3;
    }

 UINT16 message::get_data_m_param4(){
     m_param4 =  this->data[5+7]*256+this->data[5+6];
    return this->m_param4;
    }
 
 UINT32 message::get_data_m_param34(){

    
    UINT32 data=0;
    
    char * p = (char *)&data;
    char *pd =m_Afram_data.c_array();
    pd+=4;

   data=(*pd)+(*(pd+1))*256+(*(pd+2))*256*256+(*(pd+3))*256*256;
    
        return data;
    }

void message::getPRIMcheck(){
    
        UINT16 sdata1=get_data_m_param1();
        UINT16 sdata2=get_data_m_param2();
        UINT16 sdata3=get_data_m_param3();
        
        UINT16 sdata4 = sdata1^sdata2;
        sdata4 ^=sdata3;
        
        set_data_m_param4(sdata4);
    }

 void message::Set_CMD(UINT32 cmd){	
 	
	unsigned char *p = (unsigned char *)&cmd;
	set_data_m_byte(*(p+1),1);
	set_data_m_byte(*(p+2),2);
	set_data_m_byte(*(p+0),3);
}

UINT32 message::Get_CMD(){	

	UINT32 cmd=0;
	
	cmd = get_data_m_byte(2);
	cmd<<=8;
	cmd +=get_data_m_byte(1);
	cmd<<=8;
	cmd +=get_data_m_byte(3);
	
	return cmd;
}