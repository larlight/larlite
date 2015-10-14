#ifndef UBDAQID_H
#define UBDAQID_H
#include <map>
namespace ubdaq {

  class UBDaqID {
  public:
    UBDaqID() : crate(-1), card(-1), channel(-1) {};
    UBDaqID( int _crate, int _card, int _channel ) :
      crate(_crate), card(_card), channel(_channel) {};
    ~UBDaqID() {};
    
    int crate;
    int card;
    int channel;

    bool operator<(const UBDaqID& rhs) const {
      bool is_less=false;
      if (this->crate   == rhs.crate &&
          this->card    == rhs.card  &&
          this->channel <  rhs.channel) is_less=true;
      else if (this->crate == rhs.crate &&
               this->card  <  rhs.card) is_less=true;
      else if (this->crate < rhs.crate) is_less=true;
      return is_less;
    }

  };

  typedef int UBLArSoftCh_t;
  typedef std::map< UBDaqID, UBLArSoftCh_t > UBChannelMap_t;
  typedef std::map< UBLArSoftCh_t, UBDaqID > UBChannelReverseMap_t;

}
#endif
