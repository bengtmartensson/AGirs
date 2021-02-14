#pragma once



/**
 * For now, just a dummy skeleton.
 */
class IrNamedCommand {
public:
    const char* getName() const;
    const IrSignal* getIrSignal() const;
};

class IrNamedRemote {
public:
    const char* getName() const;
    const IrNamedCommand* getIrNamedCommand(const char*) const;
    size_t size() const;
    IrNamedCommand* begin() const;
    IrNamedCommand* end() const;
};

class IrNamedRemoteSet {
public:
    const IrNamedRemote* getIrNamedRemote(const char*) const;
    size_t size() const;
    IrNamedRemote* begin() const;
    IrNamedRemote* end() const;
};