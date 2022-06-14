project "zlib"
	language    	"C"
	kind        	"StaticLib"
	defines     	{ "N_FSEEKO" }
	staticruntime 	"on"
	warnings    	"off"

	files
	{
		"**.h",
		"**.c"
	}

	filter "system:windows"
		defines { "_WINDOWS" }

	filter "system:not windows"
		defines { 'HAVE_UNISTD_H' }
