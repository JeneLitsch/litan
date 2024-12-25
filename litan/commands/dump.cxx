#include "dump.hxx"
#include "../shared/shared.hxx"
#include "../shared/file.hxx"

int dump(std::span<const std::string_view> args) {
	std::string_view script_path = args[0];
	std::string_view target_path = args[1];

	std::cout << "Compiling script...\n";
	std::cout << "[Source] " << script_path << "\n";
	std::cout << "[Target] " << target_path << "\n";
	auto sst = analysis_phase(script_path);
	auto instructions = synthesis_phase(sst);
	auto ofile = open_file(target_path, std::ios::openmode(0));
	ofile << ltn::c::print(instructions);
	std::cout << "Done!\n";
	return EXIT_SUCCESS;
}
