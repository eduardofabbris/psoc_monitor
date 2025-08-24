import datetime
import os
import sys
import argparse

sys.path.insert(1, '../')
from plot_buffer import LogProcessor

# Debug
PRINT_ALL = False

# Args
parser = argparse.ArgumentParser(description="args")
parser.add_argument("filename", help="The name of the file to process")
parser.add_argument("-f", "--force", action="store_true", help="Force overwrite")
args = parser.parse_args()

log_path = args.filename

# Check log path
if not os.path.exists(log_path):
    print(f"Log file ({log_path}) not found!")
    print("exiting...")
    os._exit(0)

# Tex file
lines = []
log_name = log_path.split('.')[0].strip()
report_path =f'latex/{log_name}/'
os.makedirs(report_path, exist_ok=True)

output = report_path + f'{log_name}.tex'
if os.path.exists(output):
    print(".tex file already found!")
    if args.force:
        print("overwriting file!")
    else:
        print("exiting...")
        os._exit(0)

# Plot images
imgs_folder = 'imgs'
imgs_path = report_path + imgs_folder
os.makedirs(imgs_path, exist_ok=True)

# Log processor
proc = LogProcessor(log_path)
imgs_name = []

##########################
# Extract info from LOG
##########################

tot_monitoring_time = 0
buffer_info = []
file_buffer_num = proc.buffer_num

for i in range(0, proc.buffer_num):
    print(f"-> buffer {i}")
    buffer_info.append(proc.load_new_buffer(i, False))
    tot_monitoring_time += buffer_info[i]['device_time']

    buf = 'buffer-' + str(i) + '.png'
    imgs_name.append(buf)

    buf = imgs_path + '/' + buf
    if not os.path.exists(buf):
        print("Generating plot...")
        proc.plot_file_name = buf
        proc.plot_buffer(show_plot = False)
    else:
        print("Bypassing plot creation!")

timeout_index = 0
reset_index   = 0
buffer_index  = 0

summary_info = proc.get_summary()
header_info = proc.get_header()
timeline = proc.get_timeline()
reset_info = proc.get_resets()
timeout_info = proc.get_timeouts()
abort_info = proc.get_abort()

###################
# Page formatting
###################
lines.append("\\documentclass[12pt, a4paper]{article}")
lines.append("\\usepackage[utf8]{inputenc}")
lines.append("\\usepackage{../formatting}")
lines.append("")

###################
# Start document
###################
lines.append("\\begin{document}")
lines.append("")
lines.append("\\thispagestyle{empty}")
lines.append("\\vspace*{-1.5cm}")
lines.append("\\noindent \\textit{2025 - pt-br}")
lines.append("")

###################
# Print title
###################
info = header_info
date = datetime.datetime.fromtimestamp(info['file_timestamp'])
lines.append("\\vspace*{14pt}")
lines.append("{")
lines.append("\\fontsize{32pt}{24pt}\\selectfont{")
lines.append(f"\\begin{{center}} \\textbf{{ Analise de Resultados do Log \#{info['file_index']} da Sessao {info['file_timestamp']} }} \end{{center}}")
lines.append("}")
lines.append("}")
lines.append("\\vspace*{12pt}")
lines.append("")
lines.append("\\tableofcontents")
lines.append("\\newpage")
lines.append("")

###################
# Print summary
###################
lines.append("")
lines.append("%"*30)
lines.append("% Summary")
lines.append("%"*30)
lines.append("")

lines.append("\\addcontentsline{toc}{subsection}{Resumo}")
lines.append(f"\\section*{{Resmuo}}")
lines.append("")
lines.append(f"\\noindent \\textbf{{Nome do arquivo:}} {info['file_name']}".replace("_", "\\_"))
lines.append("")
lines.append(f"\\noindent \\textbf{{Info da sessao:}} {info['user_info']}")
lines.append("")

info = summary_info
if summary_info:
    begin_date = datetime.datetime.fromtimestamp(info['start_time'])
    end_date = datetime.datetime.fromtimestamp(info['end_time'])
    lines.append("")
    lines.append("\\begin{itemize}[leftmargin=1.3cm]")

    lines.append("")
    lines.append(f"\\item Sessao iniciada em {begin_date} e finalizada em {end_date}, resultando em um total de {info['elapsed_min']} minutos.")
    lines.append(f"\\item Tempo total de monitoramento de {round(tot_monitoring_time*1e-3/60, 2)} minutos (resultando em buffers).")
    lines.append(f"\\item O PSoC6 for reiniciado um total de {info['total_rsts']} veze(s), sendo {info['core_rsts']} core reset(s) e {info['serial_rsts']} serial reset(s).")
    lines.append(f"\\item Foi recebido um total de {info['rec_buffers']} buffers.")

    if info['rec_pckts'] != info['rec_buffers']*10 or info['rec_buffers'] != file_buffer_num or info['checksum_error'] or PRINT_ALL:
        lines.append(f"\\item Foi encontrado erro no recebimento dos buffers!")
    if info['monitor_connection_lost'] or PRINT_ALL:
        lines.append(f"\\item A conexao serial com o dispositivo watchdog foi perdida durante a sessao!")

    lines.append("")
    lines.append("\\end{itemize}")
    lines.append("")

else:
    lines.append(f"RESUMO de sessao nao encontrado!")
    lines.append(f"")

lines.append("\\addcontentsline{toc}{subsection}{Comentarios gerais}")
lines.append(f"\\section*{{Comentarios gerais}}")
lines.append("-")
lines.append("")
lines.append("")

###################
# Print timeline
###################
for event in timeline:

    match event[0]:
        # Buffer
        case "B":
            lines.append("")
            lines.append("%"*30)
            lines.append(f"% Buffer {buffer_index}")
            lines.append("%"*30)
            lines.append("")

            # Get info
            info = buffer_info[buffer_index]
            error_msg = proc.get_error_msgs(info['descriptor'])
            date = datetime.datetime.fromtimestamp(info['session_time'])
            png_file = imgs_name[buffer_index]
            png_path = os.path.join(imgs_folder, png_file)

            lines.append(f"\\section{{({date}) Recebido buffer \#{buffer_index+1}  }}")
            lines.append("")
            lines.append(f"\\textbf{{Tempo de monitoramento:}} {round(info['device_time']*1e-3 / 60, 2)} minutos")
            lines.append("")
            lines.append(f"\\textbf{{Tempo do buffer:}} {round(info['buffer_time']*1e-3, 3)} ms")
            lines.append("")
            if info['rec_pckts'] != 10 or info['checksum_error'] or info['timeout_error'] or PRINT_ALL:
                lines.append("Erro no recebimento do buffer!")
                lines.append("")

            # Plot
            lines.append("")
            lines.append("\\begin{figure}[H]")
            lines.append("\\centering")
            lines.append(f"\\includegraphics[width=1.0\\textwidth]{{ {png_path} }}")
            lines.append(f"\\caption{{ }}")
            lines.append("\\vspace{0.5em}")
            lines.append(f"\\label{{ fig:{png_file} }}")
            lines.append("\\end{figure}")
            lines.append("")

            err_smpl = info['smpl_index']
            err_smpl_num = len(err_smpl)
            err_smpl_index = "\\noindent Index da(s) amostra(s):"
            for i in range(0, min(err_smpl_num, 10)):
                err_smpl_index += f" {err_smpl[i]}"
                if i != err_smpl_num - 1 and i != 9:
                    err_smpl_index += ","

            if err_smpl_num > 10:
                err_smpl_index += " ..."

            # Errors
            lines.append("\\subsection{Erros identificados}")
            lines.append(err_smpl_index)
            lines.append("\\begin{itemize}[leftmargin=1.3cm]")
            for msg in error_msg:
                lines.append(f"\\item {msg}")
            lines.append("\\end{itemize}")
            lines.append("")

            # Comments
            lines.append("\\subsection{Comentarios}")
            lines.append("-")
            lines.append("")
            lines.append("")
            lines.append("")

            buffer_index += 1

        # Timeout
        case "t":
            lines.append("")
            lines.append("%"*30)
            lines.append(f"% Timeout {timeout_index}")
            lines.append("%"*30)
            lines.append("")

            info = timeout_info[timeout_index]
            date = datetime.datetime.fromtimestamp(info['timestamp'])
            if info['connected']:
                lines.append(f"\\section{{({date}) Conexao SERIAL reestabelecida! }}")
            else:
                lines.append(f"\\section{{({date}) Conexao SERIAL perdida! }}")

            timeout_index += 1

        # Reset
        case "r":
            lines.append("")
            lines.append("%"*30)
            lines.append(f"% Reset {reset_index}")
            lines.append("%"*30)
            lines.append("")

            info = reset_info[reset_index]
            date = datetime.datetime.fromtimestamp(info['timestamp'])
            lines.append(f"\\section{{({date}) RESET \#{reset_index} }}")
            if info['descriptor'] & 1:
                lines.append(f"Tentativa de reset \#{info['attempt']} para UART ")
            elif ( info['descriptor'] >> 7 ) & 1:
                lines.append(f"Tentativa de reset \#{info['attempt']} para CORE HANG ")

            reset_index += 1

        # Abort
        case "a":
            lines.append("")
            lines.append("%"*30)
            lines.append("% Abort")
            lines.append("%"*30)
            lines.append("")

            info = abort_info
            lines.append(f"\\section{{({date}) Sessao ABORTADA! }}")

###################
# End of document
###################
lines.append("")
lines.append("\\end{document}")

#####################
# Create .tex file
#####################
with open(output, 'w', encoding='utf-8') as f:
    f.write('\n'.join(lines))

print(f"Latex file '{output}' created with {len(imgs_name)} images.")
