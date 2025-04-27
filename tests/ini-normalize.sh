#!/bin/sh
#
#

cat $* | sed -e 's, *= *,=,' | awk -F= '
BEGIN {
	kv[0]
}
/^[#;]/ {
	next
}
/^\[/ {
	gsub(/[\]\[]/,"")
	sub(/ *$/,"")
	k1 = $0
	next
}
/=/ {
	k2 = $1
	kk = k1 "_" k2
	gsub(/[- .:/,$*]+/,"_", kk)
	kv[kk] = $2
	# print "## " $0
	# print "## " kk "=" $2
}
END {
	for (k in kv) {
		v = kv[k]
		print k "=" v
	}
}' | sort -u

