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
        self.adc_sr  = []
        self.dx      = []

####################################################################
#
# @brief  Log Processor Class
#
####################################################################
class LogProcessor:
    def __init__(self, file_name):
        self.file_name = file_name
        self.buffer_data = BufferData()
        self.en_sr : bool = False
        self.en_dx : bool = False
        self.plot_file_name : str = 'last_plot.png'
        self.buffer_num = self.get_buffer_num()
        print(f'buffers in file: {self.buffer_num}')

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
    # @brief  Extract session summary info
    # @return False if failure or summary info
    #
    def get_summary(self):
        start_found = False
        line_count  = 0
        skip_cnt    = 0
        raw_summary = []
        summary     = []

        # Open the file
        with open(self.file_name, 'r') as file:
            lines = file.readlines()

        # Find log summary
        for i, line in enumerate(lines):
            if line.startswith('* @Session summary:'):
                start_found = True

            if start_found:
                # Skip first lines
                if skip_cnt < 2:
                    skip_cnt += 1
                else:
                    line_count += 1
                    if line_count > 8:
                        break
                    raw_summary.append(line.strip())

        # Error
        if not start_found: return False

        # Extract info
        for line in raw_summary:
            aux = line.split(':')
            size = len(aux)

            if size == 2:
                aux = aux[1]
                summary.append(aux.split('*')[0].strip())
            else:
                aux = aux[1:]
                summary.append(aux[0].split('(')[0].strip())
                summary.append(aux[1].split(',')[0].strip())
                summary.append(aux[2].split(')')[0].strip())

        return {
                'start_time'              : int( summary[0] ),
                'end_time'                : int( summary[1] ),
                'elapsed_min'             : float( summary[2] ),
                'rec_buffers'             : int( summary[3] ),
                'rec_pckts'               : int( summary[4] ),
                'total_rsts'              : int( summary[5] ),
                'core_rsts'               : int( summary[6] ),
                'serial_rsts'             : int( summary[7] ),
                'checksum_error'          : int( summary[8] ),
                'monitor_connection_lost' : False if summary[9] == 'false' else True,
            }

    ##
    # @brief  Extract log file header
    # @return Header info
    #
    def get_header(self):
        start_found = False
        line_count  = 0
        raw_header  = []

        # Open the file
        with open(self.file_name, 'r') as file:
            lines = file.readlines()

        # Find log summary
        for i, line in enumerate(lines):
            if line.startswith('* @filename:'):
                start_found = True

            if start_found:
                line_count += 1
                if line_count > 4:
                    break
                raw_header.append(line.strip())

        # Error
        if not start_found: return False

        file_name = raw_header[0].split(':')[1]
        file_name = file_name.split('*')[0].strip()
        aux = file_name.split('_')
        file_date = int( aux[1].strip() )
        file_index = int( aux[2].split('.')[0].strip() )

        user_info = raw_header[3].split(':')[1]
        user_info = user_info.split('*')[0].strip()
        return {
                'file_name'      : file_name,
                'file_timestamp' : file_date,
                'file_index'     : file_index,
                'user_info'      : user_info
            }

    ##
    # @brief  Extract reset info from file
    # @return Reset info
    #
    def get_resets(self):
        rst_info = []

        # Open the file
        with open(self.file_name, 'r') as file:
            lines = file.readlines()

        # Find reset ID
        for i, line in enumerate(lines):
            if line.startswith('@r'):
                aux = line.strip()
                timestamp = aux.split(')')[0]
                timestamp = int( timestamp.split('(')[1].strip() )
                attempt = aux.split(',')[0]
                attempt = int( attempt.split('#')[1].strip() )
                descriptor = int( aux.split(':')[1].strip() )
                rst_info.append({'timestamp': timestamp, 'attempt': attempt, 'descriptor': descriptor})

        return rst_info

    ##
    # @brief  Extract timeout info from file
    # @return Timeout info
    #
    def get_timeouts(self):
        timeout_info = []

        # Open the file
        with open(self.file_name, 'r') as file:
            lines = file.readlines()

        # Find timeout ID
        for i, line in enumerate(lines):
            if line.startswith('@t'):
                aux = line.strip()
                timestamp = aux.split(')')[0]
                timestamp = int( timestamp.split('(')[1].strip() )
                connected = True if aux.split('connection')[1].strip() == 'UP' else False
                timeout_info.append({'timestamp': timestamp, 'connected': connected})

        return timeout_info

    ##
    # @brief  Extract abort info from file
    # @return Abort info
    #
    def get_abort(self):
        abort_info = None

        # Open the file
        with open(self.file_name, 'r') as file:
            lines = file.readlines()

        # Find abort ID
        for i, line in enumerate(lines):
            if line.startswith('@a'):
                aux = line.strip()
                timestamp = aux.split(')')[0]
                timestamp = int( timestamp.split('(')[1].strip() )
                abort_info = {'timestamp': timestamp}
                break

        return abort_info

    ##
    # @brief  Get log timeline
    # @return Log events timeline
    #
    def get_timeline(self):
        log_timeline = []

        # Open the file
        with open(self.file_name, 'r') as file:
            lines = file.readlines()

        # Check for lines starting with @
        for i, line in enumerate(lines):
            if line.startswith('@'):
                aux = line.strip()
                if aux[1] == 'B':
                    aux = aux[1:].split(',')[0].strip()
                else:
                    aux = aux[1:].split('(')[0].strip()
                log_timeline.append(aux)
        return log_timeline

    ##
    # @brief  Get error messages
    # @return Error message
    #
    def get_error_msgs(self, descriptor : int):
        msgs = []
        for i in range(0, len(ERROR_DES_MSGS)):
            if (descriptor >> 7 - i) & 1:
                msgs.append(ERROR_DES_MSGS[i])
        return msgs

    ##
    # @brief  Loads new buffer from file
    # @param  index        : Buffer index
    # @param  print_report : Print to console buffer report
    # @return False if failure or buffer report if print is disabled
    #
    def load_new_buffer(self, index : int = 0, print_report : bool = True):
        start_found   = False
        buffer_id     = '@B' + str(index)
        error_index   = []
        buffer        = []
        header        = []
        line_count    = 0
        skip_cnt      = 0

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
                    header.append(line.strip())
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

        # Get device timestamp
        device_timestamp = header[0].split(',')[1]
        device_timestamp = int( device_timestamp.split('-')[0].strip() )

        # Get session timestamp
        session_timestamp = header[0].split('(')[1]
        session_timestamp = int( session_timestamp.split(')')[0].strip() )

        header = header[1].split(',')

        # Get received packets
        rec_pckts = int( header[0].split(':')[1].strip() )

        # Get checksum error
        checksum_error = False if header[1].split(':')[1].strip() == 'false' else True

        # Get timeout error
        timeout_error = False if header[2].split(':')[1].strip() == 'false' else True

        # Parse buffer lines
        for i, line in enumerate(buffer):
            # Split by spaces or tabs
            column = line.split()
            if len(column) >= 4:
                try:
                    buffer_data.adc_in.append(int(column[0]))
                    buffer_data.dac_out.append(int(column[1]))
                    total_time += int(column[2])
                    if int(column[3]): error_index += [i]
                    error_des |= int(column[3])
                    buffer_data.dx +=  [abs(buffer_data.adc_in[i] - buffer_data.dac_out[i])]
                    if int(column[3]):
                        buffer_data.error_x.append(i)
                        buffer_data.error_y.append(int(column[0]))
                    if i > 0 :
                        buffer_data.adc_sr += [ abs(buffer_data.adc_in[i]  - buffer_data.adc_in[i-1]) ]
                except ValueError:
                    continue

        # Load new data set
        self.buffer_data = buffer_data

        # Print buffer status
        if print_report:
            self.print_header('Status', index)
            for err_msg in self.get_error_msgs(error_des): print(err_msg)
            print(f'Total buffer time {total_time / 1e3} ms')
        else:
            return {
                    'buffer_time'    : total_time,
                    'device_time'    : device_timestamp,
                    'session_time'   : session_timestamp,
                    'descriptor'     : error_des,
                    'smpl_index'     : error_index,
                    'rec_pckts'      : rec_pckts,
                    'checksum_error' : checksum_error,
                    'timeout_error'  : timeout_error
                }

    ##
    # @brief  Plot current loaded buffer
    # @param  xlim      : Limits for the x axis
    # @param  xtick     : Spacing for the x axis ticks
    # @param  ylim      : Limits for the y axis
    # @param  ytick     : Spacing for the y axis ticks
    # @param  show_plot : Show generated plot
    # @return None
    #
    def plot_buffer(self, xlim = [0, 1000], xtick : int = 100, ylim = [0, 4500], ytick : int = 500, show_plot : bool = True):
        """Plots the data on a graph."""
        plot_buffer = self.buffer_data
        axis_font = {'fontsize': 12, 'fontname':'Arial'}
        title_font = {'fontsize': 12, 'fontweight': 'bold', 'fontname':'Arial'}

        fig, ax = plt.subplots(figsize=(15, 5))
        ax.plot(plot_buffer.dac_out, marker='o', linestyle=' ', color='g', markersize=3, label='DAC output')
        ax.plot(plot_buffer.adc_in, marker='o', linestyle='-', color='b', markersize=3, linewidth=1.5, label='ADC input')
        ax.plot(plot_buffer.error_x, plot_buffer.error_y, marker='o', linestyle=' ', color='r', markersize=3, linewidth=1.5, label='Fault')
        if self.en_dx : ax.plot(plot_buffer.dx , marker='o', linestyle='-', color='magenta', markersize=3, linewidth=1.5, label='dx')
        if self.en_sr : ax.plot(plot_buffer.adc_sr , marker='o', linestyle='-', color='orange', markersize=3, linewidth=1.5, label='adc sr')

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

        # Save PNG image
        plt.grid(True, which='both', linestyle='--')
        fig.savefig(self.plot_file_name, dpi=400)

        # Show plot
        if show_plot:
            plt.show()
