//
// Created by Grishka on 19/03/2019.
//

#ifndef LIBTGVOIP_PACKETSENDER_H
#define LIBTGVOIP_PACKETSENDER_H

#include "VoIPController.h"
#include <functional>
#include <cstdint>

namespace tgvoip
{

class PacketSender
{
public:
    PacketSender(VoIPController* m_controller);
    virtual ~PacketSender();
    virtual void PacketAcknowledged(std::uint32_t seq, double sendTime, double ackTime, std::uint8_t type, std::uint32_t size) = 0;
    virtual void PacketLost(std::uint32_t seq, std::uint8_t type, std::uint32_t size) = 0;

protected:
    VoIPController* m_controller;

    void SendExtra(Buffer& data, std::uint8_t type) const;
    void IncrementUnsentStreamPackets();
    std::uint32_t SendPacket(VoIPController::PendingOutgoingPacket pkt);
    double GetConnectionInitTime() const;
    const HistoricBuffer<double, 32>& RTTHistory() const;
    MessageThread& GetMessageThread();
    const MessageThread& GetMessageThread() const;
    const VoIPController::ProtocolInfo& GetProtocolInfo() const;
    void SendStreamFlags(VoIPController::Stream& stm) const;
    const VoIPController::Config& GetConfig() const;
};

} // namespace tgvoip

#endif // LIBTGVOIP_PACKETSENDER_H
