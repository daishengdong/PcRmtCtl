#include "Head.h"
/**********************
*function:Format Head
*arg:*Head and content size
************************/
void Format_Head(HeadForSend *Head,u16 pkg_length){
	Head->magic1=MAGIC1;
	Head->pkg_len=pkg_length;
	Head->magic2=MAGIC2;
	#ifdef Binary
	Head->content_type=0x42;
	#endif
	#ifdef Json
	Head->content_type=0x4a;
	#endif
	#ifdef Xml
	Head->content_type=0x58;
	#endif
	Head->reserve=0x00;
}
