import os
import sys

sys.path.insert(1, '../')
from plot_buffer import LogProcessor

#log_name = 'log_1753712200_0.txt'
log_name = 'log_1753885378_0.txt'

imgs_folder = 'imgs'
imgs_path = 'latex/' + imgs_folder

os.makedirs(imgs_path, exist_ok=True)

proc = LogProcessor(log_name)
imgs_name = []

#####################
# Plot creation
#
#####################

for i in range(0, proc.buffer_num):
    print(f"-> buffer {i}")
    report = proc.load_new_buffer(i, False)
    print('-'*100)
    print(report)
    print('-'*100)
    buf = 'buffer-' + str(i) + '.png'
    imgs_name.append(buf)

    buf = imgs_path + '/' + buf
    if not os.path.exists(buf):
        print("Generating plot...")
        proc.plot_file_name = buf
        proc.plot_buffer(show_plot = False)
    else:
        print("Bypassing plot creation!")

#print('-'*100)
#print(proc.get_summary())
#print('-'*100)
#print(proc.get_header())
#print('-'*100)
#print(proc.get_timeline())
#print('-'*100)
#print(proc.get_resets())
#print(proc.get_timeouts())
#print(proc.get_abort())
#####################
# Create .tex file
#
#####################
title = 'PNG Gallery'
output='latex/main.tex'
lines = []

# Page formatting
lines.append("\\documentclass[12pt, a4paper]{article}")
lines.append("\\usepackage[utf8]{inputenc}")
lines.append("\\usepackage{formatting}")

# Start document
lines.append("\\begin{document}")
lines.append("")
lines.append("\\pagestyle{myheadings}")

# Start writing markdown content
for png_file in imgs_name:
    png_path = os.path.join(imgs_folder, png_file)
    lines.append("")
    lines.append("\\begin{figure}[H]")
    lines.append("\\centering")
    lines.append(f"\\caption{{ {png_file} }}")
    lines.append(f"\\includegraphics[width=0.9\\textwidth]{{ {png_path} }}")
    lines.append("\\vspace{0.5em}")
    lines.append(f"\\label{{ fig:{png_file} }}")
    lines.append("\\end{figure}")
    lines.append("")

# End of document
lines.append("\\end{document}")

# Write file
with open(output, 'w', encoding='utf-8') as f:
    f.write('\n'.join(lines))

print(f"Latex file '{output}' created with {len(imgs_name)} images.")
