#!/usr/bin/tclsh

# update the makefile if necessary
proc fix_Makefile { lab assign } {

if {![file exists makefile]} {
	puts stderr "No makefile found (note lower case \"m\")"
	exit
}

set data [open "makefile" r+]
set text_data [read $data]

set RE "lab$lab-[expr {$assign - 1}]$"
regsub -line $RE $text_data "lab$lab-[expr {$assign - 1}] lab$lab-$assign" text_data

set RE "\\n(lab$lab-[expr {$assign - 1}]\\s*:.*)\\nclean\\s*:"

regexp $RE $text_data foo old_rule

set RE "lab$lab-[expr {$assign -1}]"
regsub -all $RE $old_rule "lab$lab-$assign" new_rule

set RE "^lab$lab-[expr {$assign}]\\s*:.*"
if {![regexp -line $RE $text_data]} {

	regsub {\nclean :} $text_data "\n$new_rule\nclean :" text_data
}

set RE ",[expr {$assign - 1}]\}\$"
regsub -line $RE $text_data ",[expr {$assign - 1}],$assign\}" text_data

chan seek $data 0 start
puts -nonewline $data $text_data

close $data
}

proc fix_code { lab assign} {
if {![file exists lab$lab-$assign.c]} {
	puts stderr "No file lab$lab-$assign.c found!"
	exit
}

set data [open "lab$lab-$assign.c" r+]

set text_data [read $data]

set replacement "\"lab$lab-$assign.vert\", \"lab$lab-$assign.frag\""

set RE "\"lab$lab-[expr {$assign - 1}].vert\", \"lab$lab-[expr {$assign - 1}].frag\""
regsub -line $RE $text_data $replacement text_data

chan seek $data 0 start
puts -nonewline $data $text_data

close $data
}
set in_chan stdin
set out_chan stdout

set lab_nr 0
set assign_nr 0
if { $argc == 0 } {
	puts $out_chan "Input lab number (1, 2, 3 or 4)"
	while { $lab_nr != 1 && $lab_nr != 2 && $lab_nr != 3 && $lab_nr != 4} {
		gets $in_chan lab_nr
	}

	puts $out_chan "Input assignment number (ie. lab$lab_nr-X)"
	while { $assign_nr <= 0} {
		gets $in_chan assign_nr
	}
} elseif {$argc != 2} {
	puts "Wrong number of arguments!"
	puts "Go back to school!"
	exit
} 
if {$lab_nr == 0} {
	set lab_nr [lindex $argv 0]
}
if {$assign_nr == 0} {
	set assign_nr [lindex $argv 1]
}

exec cp "lab$lab_nr-[expr {$assign_nr - 1}].c" "lab$lab_nr-$assign_nr.c"

if {[file exists "lab$lab_nr-[expr {$assign_nr - 1}].vert"]} {
	exec cp "lab$lab_nr-[expr {$assign_nr - 1}].vert" "lab$lab_nr-$assign_nr.vert"
}

if {[file exists "lab$lab_nr-[expr {$assign_nr - 1}].frag"]} {
	exec cp "lab$lab_nr-[expr {$assign_nr - 1}].frag" "lab$lab_nr-$assign_nr.frag"
}

fix_Makefile $lab_nr $assign_nr
fix_code $lab_nr $assign_nr
