#! /usr/bin/perl -w
use strict;
use warnings;
use File::Spec::Functions;
use File::Find;

use Cwd;
our $startdir = getcwd;

#use Cwd 'abs_path';
#my $startabs_path = abs_path($startdir);


use File::Spec::Functions qw(
			     rel2abs
			     abs2rel

			     );

# for the convenience of &wanted calls, including -eval statements:
use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;


######################################
our @boilerplate;
# read boilerplate

open IN,"boilerplate.txt";
while (<IN>)
{
    push @boilerplate,$_;
}
close IN;

###############################

sub wanted;

my %found;

# Traverse desired filesystems
File::Find::find({wanted => \&wanted}, 
    
    'jahshaka',
    'openlibraries'

    );

use Data::Dumper;

my @copyrights = sort keys %found;

foreach my $x (@copyrights)
{
    print "\nCOPYRIGHT NOTICE :$x :\n";
    print "\n\t" . join ("\n\t",sort keys %{$found{$x}});
    print "\n";

}



sub process;

sub wanted {

#my($currentDir) = ;
    my $path =getcwd();

    my $rpath = abs2rel($path,$startdir);

    return unless -f $_;
    return if /\#\z/;

    
 #   /^.*\.cpp\z/s |
#    /^.*\.c\z/s
#    /^.*\.h\z/s
#    /^.*\.hpp\z/s
	#&& 
	process ($_,$rpath );
}

sub process
{
    my $file=shift;
    my $path=shift;
    my $count=0;
    my @source;

    open IN,"$file";

    my $state=0;

    while (<IN>)
    {
	chomp;
	s///g;

	if ($state)
	{
	    $state=0;
	    $_ = $state . $_;
	}

	if (/Copyright /i)
	{

	    if
		(

		 (/optionally output some text data/)
		 ||
		 (/Original copyright notice states that/)
		 ||
		 (/The above copyright notice and this permission notice/)
		 ||
		 (/NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE/)
		 ||
		 (/OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR/)

		 ||
		 (/following copyright and license/)

		 ||
		 (/ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE/)

		 ||
		 (/Redistributions in binary form must reproduce the above copyright notice/)

		 ||
		 (/Redistributions of source code must retain the above copyright notice/)


		 ||
		 (/Redistributions of source code must retain the above copyright notice/)

		 ||
		 (/THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS/)



		 )
	    {

	    }
	    else
	    {
		if (/\/\/ Copyright (C) 2005-2006 Editopia Inc./)
		{
		    push  @source,@boilerplate;
		}

		if (/\d+\s*$/)
		{
		    $state=$_;
		}
		elsif (/\d+\,\s*$/)
		{
		    $state=$_;
		}
		else
		{
		    if (!$found{$_})
		    {
#		    warn "other :$_";
		    }
		    $found{$_}{$path. $file}++;
		}
	    }
	}
	push @source,$_;
    }
    close IN;

    if ($count)
    {
	#found something
    }
    else
    {
	push @source, "
/*-*-CopyLeft-*-

          Warning : No Copyright Assignment Found in Original Source.

-*-CopyLeft-*-*/

";
	push @source,@boilerplate;
	$found{"NONE"}{$path. $file}++
	}
    #print @source;
    
}



