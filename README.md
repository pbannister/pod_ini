# POD configuration
Aim of this exercise is to ingest configuration files in (Windows-like) .ini-style (or Java-style .properties) format.
In past, I have written code to ingest either (interchangably) using reflection to map into native Java (or Javascript, or Python) data structures. 
Using C/C++ does not allow the same.

Flip this around.

Contents of configuration files is only meaningful if ingested by source code.
So source code defines what should be in a configuration file.
(Ignoring the unusual case where a configuration file might be shared by independent software.)
Configuration read into POD (simple direct C++ data structure) is simplest to access.

So ... could ingest example configuration files:
1.  Generate the C++ code to ingest to POD.
2.  Generate the C++ code to generate example/default configuration files from POD.
3.  (Perhaps) generate an intermediate representation, from which to generate C/C++ code.

## POD racer
(Yeh. I just liked the name.)

First task was to ingest structured configuration files (either/both **ini** or **properties**).

The example code ingests the entire configuration file in a single ``read()``,
then optimally splits key/values out of that block.

The next task is to generate C structures, then code to read into those structures.

## Structured storage and common representation
INI files are grouped by ``[section]`` with associated ``key=value`` assignments.

PROPERTIES files are both more and less structured, with just ``us.bannister.key=value`` assignments.
Keys are structured like (reversed) Internet host names (a Java convention).
In languages that support reflection, those dotted-names can be mapped into native data structures.

You can unify both formats by treating INI files as yielding ``section.key=value``.

Conversely, PROPERTIES can become ``[us.bannister]`` and ``key=value``.
So a single reader can accept both formats.




