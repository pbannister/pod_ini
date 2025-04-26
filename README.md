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



## Structured naming
As an aside, my approach to naming might be unfamiliar.

With the usual CamelCase naming (and yes, I date before that was new), 
in a smart IDE, the grouping of members is a bit random.
This bothered me.

Over time, I noticed that classes tended to have *facets*.
When those facets were separable, they might turn into *interfaces*.
But rather often the facets of an object were more dependent, and not truly separable from the class.
(Should mention "mixins" from the Lisp world, but pre-web so links are scarce.)

You might also note that I use a minimalist version of "Hungarian" notation.
(And yes, I spent time in Win16, where this got a bit extreme, of need.)

The minimalist end is:
* Method names with the most significant bit first. Something like: ```facet_subfacet_operation()```
* Class names end with "_o".
* Object names start with "o_" (mostly).
* Pointer names start with "p_".
* Index names start with "i_".
* Count names start with "n_".
* String names start with "s_" (mostly).
* Array names start with "a_" (sometimes).

Not remotely religious about any of the above, but works for me.
