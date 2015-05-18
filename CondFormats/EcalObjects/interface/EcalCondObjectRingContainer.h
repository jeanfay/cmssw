#ifndef ECAL_COND_OBJECT_RING_CONTAINER_HH
#define ECAL_COND_OBJECT_RING_CONTAINER_HH

#include "CondFormats/Serialization/interface/Serializable.h"

#include "DataFormats/EcalDetId/interface/EcalRingContainer.h"

template < typename T >
class EcalCondObjectRingContainer {
        public:
                typedef T Item;
                typedef Item value_type;
                typedef EcalCondObjectRingContainer<T> self;
                typedef typename std::vector<Item> Items;
                typedef typename std::vector<Item>::const_iterator const_iterator; 
                typedef typename std::vector<Item>::iterator iterator;

                EcalCondObjectRingContainer() {};
                ~EcalCondObjectRingContainer() {};

                inline
                const Items & barrelItems() const { return eb_.items(); };

                inline
                const Items & endcapItems() const { return ee_.items(); };

                inline
                const Item & barrel( short ringid ) const {
                        return eb_.item(ringid);
                }
                
                inline
                const Item & endcap( short ringid ) const {
                        return ee_.item(ringid);
                }

                inline
                void insert( std::pair<short, Item> const &a ) {

                        if (a.first < N_RING_BARREL) {
                            eb_.insert(a);
                        } else if (a.first >= N_RING_BARREL && a.first < N_RING_TOTAL) {                 
                             ee_.insert(a);
                        } else return;
                        
                }
                
                inline
                const_iterator find( short ringid ) const {

                        if (ringid < N_RING_BARREL) {
                            const_iterator it = eb_.find(ringid);
                            if ( it != eb_.end() ) {
                                 return it;
                            } else {
                                 return ee_.end();
                            }
                        } else if (ringid >= N_RING_BARREL && ringid < N_RING_TOTAL) {                 
                             return ee_.find(ringid);
                        } else return ee_.end();

                }

                inline
                const_iterator begin() const {
                        return eb_.begin();
                }

                inline
                const_iterator end() const {
                        return ee_.end();
                }

                inline
                void setValue(const short id, const Item &item) {
                        (*this)[id] = item;
                }

                inline
                const self & getMap() const {
                        return *this;
                }

                inline
                size_t size() const {
                        return eb_.size() + ee_.size();
                }
                // add coherent operator++, not needed now -- FIXME

                inline
                Item & operator[]( short ringid ) 
                {
                        return (ringid < N_RING_BARREL) ? eb_[ringid] : ee_[ringid];
                }

                inline
                Item operator[]( short ringid ) const {

                        if (ringid < N_RING_BARREL) {
                            return eb_[ringid];
                        } else if (ringid >= N_RING_BARREL && ringid < N_RING_TOTAL) {                 
                             return ee_[ringid];
                        } else return Item();

                }
                
        private:
                
                EcalRingContainer< short, Item > eb_;
                EcalRingContainer< short, Item > ee_;

        COND_SERIALIZABLE;
};

typedef EcalCondObjectRingContainer<float> EcalFloatCondObjectRingContainer;
#endif
