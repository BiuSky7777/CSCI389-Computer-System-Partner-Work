
#include "fifo_evictor.hh"

FIFO_Evictor::FIFO_Evictor() {
    queue<key_type> myqueue;
}

void FIFO_Evictor::touch_key(const key_type& key){
  myqueue.push(key);
}

const key_type  FIFO_Evictor::evict() {
  key_type poped = myqueue.front();
  myqueue.pop();
  return poped;
}
