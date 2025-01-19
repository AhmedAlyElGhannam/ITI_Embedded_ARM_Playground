def calculate_pll_prescalers(input_freq_mhz, target_freq_mhz=84):
    """
    Calculates PLL prescaler values M, N, P, and Q for STM32F401CC.
    :param input_freq_mhz: Input clock frequency in MHz (e.g., HSE frequency)
    :param target_freq_mhz: Target system clock frequency in MHz (default: 84 MHz)
    :return: Dictionary with M, N, P, and Q values
    """
    # Constraints from the STM32F401 reference manual
    M_range = range(2, 64)  # M must be between 2 and 63
    N_range = range(50, 433)  # N must be between 50 and 432
    P_values = [2, 4, 6, 8]  # P must be one of these values
    Q_range = range(2, 16)  # Q must be between 2 and 15

    for M in M_range:
        vco_input = input_freq_mhz / M
        if 1 <= vco_input <= 2:  # VCO input frequency must be between 1 and 2 MHz
            for N in N_range:
                vco_output = vco_input * N
                if 100 <= vco_output <= 432:  # VCO output frequency must be between 100 and 432 MHz
                    for P in P_values:
                        system_clock = vco_output / P
                        if abs(system_clock - target_freq_mhz) < 1e-3:  # Check if system clock matches target
                            for Q in Q_range:
                                usb_clock = vco_output / Q
                                if 48 - 0.1 <= usb_clock <= 48 + 0.1:  # USB clock must be 48 MHz
                                    return {"M": M, "N": N, "P": P, "Q": Q}
    return None

def calculate_bus_prescalers(system_clock_mhz):
    """
    Calculates AHB and APB prescalers for STM32F401CC.
    :param system_clock_mhz: System clock frequency in MHz
    :return: Dictionary with AHB, APB1, and APB2 prescaler values
    """
    AHB_prescalers = [1, 2, 4, 8, 16, 64, 128, 256, 512]
    APB_prescalers = [1, 2, 4, 8, 16]

    for AHB in AHB_prescalers:
        ahb_freq = system_clock_mhz / AHB
        if ahb_freq <= 84:  # Max AHB clock is 84 MHz
            for APB1 in APB_prescalers:
                apb1_freq = ahb_freq / APB1
                if apb1_freq <= 42:  # Max APB1 clock is 42 MHz
                    for APB2 in APB_prescalers:
                        apb2_freq = ahb_freq / APB2
                        if apb2_freq <= 84:  # Max APB2 clock is 84 MHz
                            return {"AHB": AHB, "APB1": APB1, "APB2": APB2}
    return None

def calculate_plli2s_prescalers(input_freq_mhz, target_audio_freq_mhz):
    """
    Calculates PLLI2S prescaler values R and N for STM32F401CC.
    :param input_freq_mhz: Input clock frequency in MHz (e.g., HSE frequency)
    :param target_audio_freq_mhz: Target audio frequency in MHz
    :return: Dictionary with N and R values
    """
    N_range = range(50, 433)  # N must be between 50 and 432
    R_range = range(2, 8)  # R must be between 2 and 7

    for N in N_range:
        for R in R_range:
            pll_i2s_clock = (input_freq_mhz * N) / R
            if abs(pll_i2s_clock - target_audio_freq_mhz) < 1e-3:
                return {"N": N, "R": R}
    return None

# Example usage
if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Calculate STM32F401CC clock prescalers.")
    parser.add_argument("input_freq", type=float, help="Input clock frequency in MHz (e.g., HSE frequency)")
    parser.add_argument("audio_freq", type=float, help="Target audio clock frequency in MHz", nargs="?", default=48)

    args = parser.parse_args()

    input_freq = args.input_freq
    audio_freq = args.audio_freq

    # Calculate PLL prescalers
    pll_prescalers = calculate_pll_prescalers(input_freq)
    if pll_prescalers:
        print("PLL Prescalers:", pll_prescalers)

        # Calculate bus prescalers
        bus_prescalers = calculate_bus_prescalers(84)
        if bus_prescalers:
            print("Bus Prescalers:", bus_prescalers)

        # Calculate PLLI2S prescalers
        plli2s_prescalers = calculate_plli2s_prescalers(input_freq, audio_freq)
        if plli2s_prescalers:
            print("PLLI2S Prescalers:", plli2s_prescalers)
        else:
            print("Could not calculate PLLI2S prescalers.")
    else:
        print("Could not calculate PLL prescalers.")

