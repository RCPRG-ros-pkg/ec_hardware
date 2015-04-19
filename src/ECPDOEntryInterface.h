#ifndef PDOENTRYINTERFACE_H
#define PDOENTRYINTERFACE_H

class ECPDOEntryInterface {
public:
  ECPDOEntryInterface(uint16_t index, uint8_t subindex):
    index_(index),
    subindex_(subindex),
    offset_(0),
    domain_ptr_(0) {
  }
  
  const uint16_t getIndex() const{
    return index_;
  }
  
  const uint8_t getSubIndex() const{
    return subindex_;
  }
  
  unsigned int *getOffsetPtr() {
    return &offset_;
  }

  void setDomainPtr(uint8_t *ptr) {
    domain_ptr_ = ptr;
  }

protected:
  const uint16_t index_;
  const uint8_t subindex_;
  unsigned int offset_;
  uint8_t *domain_ptr_;

private:
  ECPDOEntryInterface() : index_(0), subindex_(0), offset_(0), domain_ptr_(0) {
  }
};

#endif
