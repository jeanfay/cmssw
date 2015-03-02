#ifndef ECALDETID_ECALRINGCONTAINER_H
#define ECALDETID_ECALRINGCONTAINER_H

#include <vector>
#include <utility>
#include <algorithm>


// #include <iostream>



/* a generic container for ecal items
 * provides access by ringId...
 */

static const short N_RING_TOTAL = 248;
static const short N_RING_BARREL = 170;
static const short N_RING_ENDCAP = 78;

template<typename DetId, typename T>
class EcalRingContainer {

        public:

                typedef EcalRingContainer<DetId, T> self;
                typedef T Item;
                typedef Item value_type;
                typedef typename std::vector<Item> Items; 
                typedef typename std::vector<Item>::const_iterator const_iterator;
                typedef typename std::vector<Item>::iterator iterator;

                EcalRingContainer() {checkAndResize();}

                void insert(std::pair<short, Item> const &a) {
                        (*this)[a.first] = a.second;
                }

                inline const Item & item(short ringid) const {
                        return m_items[ringid];
                }

                inline const Items & items() const {
                        return m_items;
                }

                inline Item & operator[](short ringid) {
		  checkAndResize();
		  static Item dummy;
		  if ( !isValidId(ringid) ) return dummy;
		  return m_items[ringid];
                }

                void checkAndResize() {
		  if (m_items.size()==0) {
		    m_items.resize(N_RING_TOTAL);
		  }
		}


		void checkAndResize( size_t priv_size ) {
		  // this method allows to resize the vector to a specific size forcing a specific value
		  if (m_items.size()==0) {
		    m_items.resize(priv_size);
		  }
		}


                inline Item const & operator[](short ringid) const {
                        if ( !isValidId(ringid) ) return dummy_item();
                        return m_items[ringid];
                }

                inline const_iterator find(short ringid) const {
                        if ( !isValidId(ringid) ) return m_items.end();
                        return m_items.begin() + ringid;
                }

                inline const_iterator begin() const {
                        return m_items.begin();
                }

                inline const_iterator end() const {
                        return m_items.end();
                }

                inline size_t size() const {
                        return m_items.size();
                }

                void setItems(const std::vector<Item> & items) {
                    m_items = items;
                }

        private:

		//Cint can't parse the new C++11 initialization syntax
#if !defined(__CINT__) && !defined(__MAKECINT__) && !defined(__REFLEX__)

		static const Item& dummy_item() {
		  static const Item s_dummy{};
		  return s_dummy;
		}
#else
		static const Item& dummy_item();
#endif
                inline bool isValidId(const short ringid) const {
                        if(ringid >= N_RING_TOTAL) return false;
                        else return true;
                };

                std::vector<Item> m_items;

};



#endif // ECALRINGCONTAINER
