#ifndef Encoder_h_
#define Encoder_h_

#include <gmock-wrapper.h>

class EncoderMock {
  public:
    MOCK_CONST_METHOD0(read, long());
};

class Encoder {
  public:
    Encoder(int, int) : mock{nullptr} {}
    Encoder(const EncoderMock &mock) : mock(&mock) {}
    Encoder(const Encoder &) = default;
    long read() const { return mock ? mock->read() : 0; }

  private:
    const EncoderMock *mock;
};

#endif