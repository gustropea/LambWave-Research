from graphviz import Source
import subprocess

graph_name = 'diagram'
s = Source.from_file(graph_name+'.dot')
s.render(graph_name,format='pdf',view=True)
p2 = subprocess.Popen(["rm "+graph_name], stdout=subprocess.PIPE, shell=True)
p2.communicate()
p2.wait()
