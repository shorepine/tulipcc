
{%- extends 'display_priority.tpl' -%}


{% block in_prompt %}
{% endblock in_prompt %}

{% block output_prompt %}
{% endblock output_prompt %}

{% block input scoped%}

{%- if cell.source.split('\n')[0].startswith('%%micropython') -%}
.. code::
        
{{ '\n'.join(['# code to be run in micropython'] + cell.source.strip().split('\n')[1:]) | indent}}

{%- else -%}
.. code::

{{ '\n'.join(['# code to be run in CPython\n'] + cell.source.strip().split('\n')) | indent}}
{%- endif -%}
{% endblock input %}

{% block error %}
::

{{ super() }}
{% endblock error %}

{% block traceback_line %}
{{ line | indent | strip_ansi }}
{% endblock traceback_line %}

{% block execute_result %}
{% block data_priority scoped %}
{{ super() }}
{% endblock %}
{% endblock execute_result %}

{% block stream %}
.. parsed-literal::

{{ output.text | indent }}
{% endblock stream %}

{% block data_svg %}
.. image:: {{ output.metadata.filenames['image/svg+xml'] | urlencode }}
{% endblock data_svg %}

{% block data_png %}
.. image:: {{ output.metadata.filenames['image/png'] | urlencode }}
{%- set width=output | get_metadata('width', 'image/png') -%}
{%- if width is not none %}
   :width: {{ width }}px
{%- endif %}
{%- set height=output | get_metadata('height', 'image/png') -%}
{%- if height is not none %}
   :height: {{ height }}px
{%- endif %}
{% endblock data_png %}

{% block data_jpg %}
.. image:: {{ output.metadata.filenames['image/jpeg'] | urlencode }}
{%- set width=output | get_metadata('width', 'image/jpeg') -%}
{%- if width is not none %}
   :width: {{ width }}px
{%- endif %}
{%- set height=output | get_metadata('height', 'image/jpeg') -%}
{%- if height is not none %}
   :height: {{ height }}px
{%- endif %}
{% endblock data_jpg %}

{% block data_markdown %}
{{ output.data['text/markdown'] | convert_pandoc("markdown", "rst") }}
{% endblock data_markdown %}

{% block data_latex %}
.. math::

{{ output.data['text/latex'] | strip_dollars | indent }}
{% endblock data_latex %}

{% block data_text scoped %}
.. parsed-literal::

{{ output.data['text/plain'] | indent }}
{% endblock data_text %}

{% block data_html scoped %}
.. raw:: html

{{ output.data['text/html'] | indent }}
{% endblock data_html %}

{% block markdowncell scoped %}
{{ cell.source | convert_pandoc("markdown", "rst") }}
{% endblock markdowncell %}

{%- block rawcell scoped -%}
{%- if cell.metadata.get('raw_mimetype', '').lower() in resources.get('raw_mimetypes', ['']) %}
{{cell.source}}
{% endif -%}
{%- endblock rawcell -%}

{% block headingcell scoped %}
{{ ("#" * cell.level + cell.source) | replace('\n', ' ') | convert_pandoc("markdown", "rst") }}
{% endblock headingcell %}

{% block unknowncell scoped %}
unknown type  {{cell.type}}
{% endblock unknowncell %}
