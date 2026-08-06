from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "recomendacao",
        [
            "src/binding.cpp",
            "src/similaridade.cpp",
            "src/recomendacao.cpp",
        ],
        include_dirs=["include"],
        cxx_std=17,
    ),
]

setup(
    name="recomendacao",
    version="0.2.0",
    description="Integração Python + C++ (pybind11) do Sistema de Recomendação com matrizes esparsas CSR",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)