#include <iostream>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>

namespace py = pybind11;

class __attribute__((visibility("default"))) RunPython {
public:
    RunPython(const std::string& ModulePath) : _ModulePath(ModulePath), guard{} {
        try {
            py::module sys = py::module::import("sys");
            sys.attr("path").cast<py::list>().append(ModulePath.c_str());
        }
        catch (const py::error_already_set& e) {
            std::cerr << "Error initializing Python or setting path: " << e.what() << std::endl;
        }
    }

   
    void LoadModule(const std::string& ModuleName) {
        try {
            _Module = py::module::import(ModuleName.c_str());
        }
        catch (const py::error_already_set& e) {
            std::cerr << "Error loading module '" << ModuleName << "': " << e.what() << std::endl;
        }
    }

    
    void Function(const std::string& FunctionName, int Parameter1, int Parameter2) {
        if (!_Module) {
            std::cerr << "Error: Module not loaded!" << std::endl;
            return;
        }

        try {
            _Results = _Module.attr(FunctionName.c_str())(Parameter1, Parameter2);
        }
        catch (const py::error_already_set& e) {
            std::cerr << "Error calling function '" << FunctionName << "': " << e.what() << std::endl;
        }
    }

   
    py::object GetResults() const {
        return _Results;
    }

   
    void PrintResults() const {
        if (_Results) {
            try {
                std::cout << "Result: " << _Results.cast<int>() << std::endl;
            }
            catch (const py::cast_error& e) {
                std::cerr << "Error casting result to int: " << e.what() << std::endl;
                try {
                    std::cout << "Result (as string): " << _Results.cast<std::string>() << std::endl;
                }
                catch (const py::cast_error& e) {
                    std::cerr << "Error casting result to string: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "No result available." << std::endl;
        }
    }

   
    std::string GetModulePath() const {
        return _ModulePath;
    }

    void SetModulePath(const std::string& NewModulePath) {
        _ModulePath = NewModulePath;
    }

private:
    py::scoped_interpreter guard;  // ��ʼ���͹���Python������
    std::string _ModulePath;
    py::module _Module;            // �洢Pythonģ��
    py::object _Results;           // �洢Python�����ķ��ؽ��
};
