#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
extern "C"
void __cxa_pure_virtual [[gnu::weak]](void){
  }
extern "C" 
int strcmp [[gnu::pure]](const char* x,const char* y){
  int c=*x-*y;
  while (c==0 && *x!=0){
    ++x;++y;
    c = *x - *y;
    }
  return c;
  }
extern "C" 
unsigned long strlen [[gnu::pure]](const char* x){
  unsigned long c=0;
  while (*(x+c)!=0) ++c;
  return c;
  }
extern "C" 
void* memcpy [[gnu::pure]](void* __restrict x,const void* __restrict y,long unsigned count){
  long unsigned i=0;
  while (i<count){
    static_cast<unsigned char*>(x)[i]=static_cast<const unsigned char*>(y)[i];
    ++i;
    }
  return x;
  }
extern "C" 
void* memmove [[gnu::pure]](void* x,const void* y,long unsigned count){
  long unsigned i=0;
  while (i<count){
    static_cast<unsigned char*>(x)[i]=static_cast<const unsigned char*>(y)[i];
    ++i;
    }
  return x;
  }
extern "C" 
void* memset [[gnu::pure]](void* x,int val,long unsigned count){
  auto c = static_cast<unsigned char>(val);
  long unsigned i=0;
  while (i<count){
    static_cast<unsigned char*>(x)[i]=c;
    ++i;
    }
  return x;
  }
#pragma GCC diagnostic pop
