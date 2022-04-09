#include <fstream>
#include <filesystem>
#include "ltnvm/LtnVM.hxx"
#include "ltnvm/external/External.hxx"

class Test : public ltn::vm::ext::External {
public:
	Test() : External(2) {}
	virtual void operator()(ltn::vm::ext::Api & api) override {
		const auto a = api.parameter<std::string>(0); 
		const auto b = api.parameter<std::string>(1); 
		std::cout << a << "|" << b << "\n";
		api.return_string("Hello World");
	}
};

int main(int argc, char const *argv[]) {
	if(argc > 1) {
		if(std::filesystem::exists(argv[1])) {
			std::ifstream file(argv[1], std::ios::binary);
			std::vector<std::uint8_t> bytecode{
				std::istreambuf_iterator<char>(file),
				std::istreambuf_iterator<char>()
			};
			std::vector<std::string> args;
			for(std::size_t i = 2; i < argc; ++i) {
				args.push_back(argv[i]);
			}
			try {
				ltn::vm::LtnVM vm{std::cout};
				vm.register_external(42, std::make_unique<Test>());
				vm.setup(bytecode);
				vm.run(args);
				return EXIT_SUCCESS;
			}
			catch(const std::runtime_error & error) {
				std::cout << "[VM-Error] " << error.what() << "\n";
				return EXIT_FAILURE;
			}
		}
		std::cout << "[VM-Error] Cannot open: " << argv[1] << "\n";
	}
	std::cout << "[VM-Error] Needs a file to run\n";
	return EXIT_FAILURE;
}
