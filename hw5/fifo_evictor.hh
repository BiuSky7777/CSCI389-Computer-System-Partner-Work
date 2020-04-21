
#include "evictor.hh"
#include <queue>
using namespace std;


class FIFO_Evictor: public Evictor {

 private:

  queue<key_type> myqueue;

 public:

   FIFO_Evictor();

   ~FIFO_Evictor() = default;

   virtual void touch_key(const key_type& key) ;

   virtual const key_type evict();

};
