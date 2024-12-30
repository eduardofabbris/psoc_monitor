###################################################################################
#                                  Error Detection System
###################################################################################
"""
@file       plot_buffer.py
@author     Eduardo Fabbris
@brief      Buffer handling functions
"""
####################################################################################
#
# MIT License
#
# Copyright (c) 2024 eduardofabbris
# See the LICENSE file for details.
#
####################################################################################
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

# Error descriptor
ERROR_DES_MSGS =\
[
    "ADC input mismatch in backup buffers",
    "DAC output mismatch in backup buffers",
    "Time difference mismatch in backup buffers",
    "Time difference outside tolerated range",
    "",
    "",
    "Slew-rate outside tolerated range",
    "Written and read difference outside tolerated range",
]

class BufferData:
    def __init__(self):
        self.adc_in  = []
        self.dac_out = []
        self.error_x = []
        self.error_y = []

####################################################################
#
# @brief  Log Processor Class
#
####################################################################
class LogProcessor:
    def __init__(self, file_name):
        self.file_name = file_name
        self.buffer_data = BufferData()
        print(f'buffers in file: {self.get_buffer_num()}')

    def help(self): help(self)

    ##
    # @brief  Print status header
    # @param  index : Buffer index
    #
    def print_header(self, msg: str, index: int = None):
        new_msg = f"   {msg} from buffer #{index}    " if index is not None else f"   {msg}   "
        msg_len = len(new_msg)
        print("-" * msg_len)
        print(new_msg)
        print("-" * msg_len)

    ##
    # @brief  Get the number of buffers in file
    # @return Buffer count
    #
    def get_buffer_num(self):
        buffer_cnt = 0

        # Open the file
        with open(self.file_name, 'r') as file:
            lines = file.readlines()

        # Check for lines starting with @B
        for i, line in enumerate(lines):
            if line.startswith('@B'):
                buffer_cnt += 1

        return buffer_cnt

    ##
    # @brief  Loads new buffer from file
    # @param  index : Buffer index
    # @return False if failure
    #
    def load_new_buffer(self, index : int = 0):
        start_found = False
        buffer_id = '@B' + str(index)
        buffer      = []
        line_count  = 0
        skip_cnt    = 0

        # Open the file
        with open(self.file_name, 'r') as file:
            lines = file.readlines()

        # Get buffer in file
        for i, line in enumerate(lines):

            # Check for lines starting with buffer identifier
            if line.startswith(buffer_id):
                start_found = True

            if start_found:
                # Skip header
                if skip_cnt < 2:
                    skip_cnt += 1
                else:
                    line_count += 1
                    if line_count > 1000:
                        break
                    # Store the line in buffer
                    buffer.append(line.strip())

        if not start_found:
            print('Buffer not found!')
            return False

        # Prepare data for plotting
        buffer_data = BufferData()
        total_time = 0
        error_des = 0

        # Parse buffer lines
        for i, line in enumerate(buffer):
            # Split by spaces or tabs
            column = line.split()
            if len(column) >= 4:
                try:
                    buffer_data.adc_in.append(int(column[0]))
                    buffer_data.dac_out.append(int(column[1]))
                    total_time += int(column[2])
                    error_des |= int(column[3])
                    if int(column[3]):
                        buffer_data.error_x.append(i)
                        buffer_data.error_y.append(int(column[0]))
                except ValueError:
                    continue

        # Print buffer status
        self.print_header('Status', index)
        for i in range(0, len(ERROR_DES_MSGS)):
            if (error_des >> 7 - i) & 1:
                print(ERROR_DES_MSGS[i])

        print(f'Total buffer time {total_time / 1e3} ms')

        # Load new data set
        self.buffer_data = buffer_data

    ##
    # @brief  Plot current loaded buffer
    # @param  xlim      : Limits for the x axis
    # @param  xtick     : Spacing for the x axis ticks
    # @param  ylim      : Limits for the y axis
    # @param  ytick     : Spacing for the y axis ticks
    # @param  file_name : File name for png save
    # @return None
    #
    def plot_buffer(self, xlim = [0, 1000], xtick : int = 100, ylim = [0, 4500], ytick : int = 500, file_name : str = 'last_plot.png'):
        """Plots the data on a graph."""
        plot_buffer = self.buffer_data
        axis_font = {'fontsize': 12, 'fontname':'Arial'}
        title_font = {'fontsize': 12, 'fontweight': 'bold', 'fontname':'Arial'}

        fig, ax = plt.subplots(figsize=(10, 5))
        ax.plot(plot_buffer.dac_out, marker='o', linestyle=' ', color='g', markersize=3, label='DAC output')
        ax.plot(plot_buffer.adc_in, marker='o', linestyle='-', color='b', markersize=3, linewidth=1.5, label='ADC input')
        ax.plot(plot_buffer.error_x, plot_buffer.error_y, marker='o', linestyle=' ', color='r', markersize=3, linewidth=1.5, label='Fault')

        # Labels
        ax.set_title(' ', fontdict=title_font)
        ax.set_xlabel('Samples', fontdict=axis_font)
        ax.set_ylabel('Amplitude', fontdict=axis_font)
        ax.legend(loc='upper right')

        # Set axis limits
        ax.set_xlim(xlim[0], xlim[1])
        ax.set_ylim(ylim[0], ylim[1])

        # Set ticks spacing
        ax.xaxis.set_major_locator(MultipleLocator(xtick))
        ax.yaxis.set_major_locator(MultipleLocator(ytick))

        plt.grid(True, which='both', linestyle='--')
        fig.savefig(file_name, dpi=400)
        plt.show()

