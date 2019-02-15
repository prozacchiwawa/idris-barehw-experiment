int peek8(uint64_t addr) {
    return *((uint8_t *)addr);
}

int peek16(uint64_t addr) {
    return *((uint16_t *)addr);
}

int peek32(uint64_t addr) {
    return *((uint32_t *)addr);
}

int peek64(uint64_t addr) {
    return *((uint64_t *)addr);
}

void poke8(uint64_t addr, uint8_t val) {
    *((uint8_t *)addr) = val;
}

void poke16(uint64_t addr, uint16_t val) {
    *((uint16_t *)addr) = val;    
}

void poke32(uint64_t addr, uint32_t val) {
    *((uint32_t *)addr) = val;    
}

void poke64(uint64_t addr, uint64_t val) {
    *((uint64_t *)addr) = val;    
}
