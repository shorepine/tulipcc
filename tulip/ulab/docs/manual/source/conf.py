# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

#import sphinx_rtd_theme

from sphinx.transforms import SphinxTransform
from docutils import nodes
from sphinx import addnodes

# -- Project information -----------------------------------------------------

project = 'The ulab book'
copyright = '2019-2024, Zoltán Vörös and contributors'
author = 'Zoltán Vörös'

# The full version, including alpha/beta/rc tags
release = '6.6.0'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

latex_maketitle = r'''
\begin{titlepage}
\begin{flushright}
\Huge\textbf{The $\mu$lab book}
\vskip 0.5em
\LARGE
\textbf{Release %s}
\vskip 5em
\huge\textbf{Zoltán Vörös}
\end{flushright}
\begin{flushright}
\LARGE
\vskip 2em
with contributions by
\vskip 2em
\textbf{Roberto Colistete Jr.}
\vskip 0.2em
\textbf{Jeff Epler}
\vskip 0.2em
\textbf{Taku Fukada}
\vskip 0.2em
\textbf{Diego Elio Pettenò}
\vskip 0.2em
\textbf{Scott Shawcroft}
\vskip 5em
\today
\end{flushright}
\end{titlepage}
'''%release

latex_elements = {
    'maketitle': latex_maketitle
}


master_doc = 'index'

author=u'Zoltán Vörös'
copyright=author
language='en'

latex_documents = [
(master_doc, 'the-ulab-book.tex', 'The $\mu$lab book',
'Zoltán Vörös', 'manual'),
]

# Read the docs theme
on_rtd = os.environ.get('READTHEDOCS', None) == 'True'
if not on_rtd:
    try:
        import sphinx_rtd_theme
        html_theme = 'sphinx_rtd_theme'
        html_theme_path = [sphinx_rtd_theme.get_html_theme_path(), '.']
    except ImportError:
        html_theme = 'default'
        html_theme_path = ['.']
else:
    html_theme_path = ['.']
