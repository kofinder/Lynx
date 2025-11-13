#include <variant>
#include <vector>
#include <iostream>
#include "ArrayInitializerNode.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;
using namespace LynxContext;
using namespace LynxConstants;
        
namespace LynxAst {

    llvm::Value* ArrayInitializerNode::generateCode(std::shared_ptr<AstContext> astContext) {
        llvm::Value* value = nullptr;
        switch (this->arrayType) {
            case DataType::BOOLEAN:
                value = this->generateBooleanArray(astContext);
                break;
            case DataType::BYTE:
                value = this->generateByteArray(astContext);
                break;
            case DataType::SHORT:
                value = this->generateShortArray(astContext);
                break;
            case DataType::INT:
                value = this->generateIntegerArray(astContext);
                break;
            case DataType::LONG:
                value = this->generateLongArray(astContext);
                break;
            case DataType::FLOAT:
                value = this->generateFloatArray(astContext);
                break;
            case DataType::DOUBLE:
                value = this->generateDoubleArray(astContext);
                break;
            case DataType::STRING:
                value = this->generateStringArray(astContext);
                break;
            case DataType::CHAR:
                value = this->generateCharArray(astContext);
                break;
            case DataType::VOID:
            case DataType::ARRAY:
            case DataType::ENUM:
            default: break;
        }

        return value;
    }

    llvm::Value* ArrayInitializerNode::generateBooleanArray(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Generate Boolean Array ............");
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if (singleDimArray) {

            const auto& boolVector = std::get<std::vector<bool>>(*singleDimArray);
            std::vector<uint8_t> boolArrayConverted(boolVector.begin(), boolVector.end());

            llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt1Ty(context), boolVector.size());

            llvm::Constant* arrayConst = llvm::ConstantDataArray::get(context, boolArrayConverted);

            llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, "booleanArray");
            globalArray->setAlignment(llvm::Align(1));

            llvm::Value* globalArrayPtr = builder.CreateBitCast(globalArray, llvm::PointerType::get(context, 0));
            return globalArrayPtr;
        } else {
            const auto& boolMatrix = std::get<std::vector<std::vector<bool>>>(*multiDimArray);
            size_t numRows = boolMatrix.size();
            size_t numCols = boolMatrix.front().size();
            llvm::ArrayType* rowType = llvm::ArrayType::get(llvm::Type::getInt1Ty(context), numCols);
            llvm::ArrayType* matrixType = llvm::ArrayType::get(rowType, numRows);

            std::vector<llvm::Constant*> llvmRows;
            for (const auto& row : boolMatrix) {
                std::vector<llvm::Constant*> llvmBoolValues;
                for (bool val : row) {
                    llvmBoolValues.push_back(llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), val));
                }
                llvmRows.push_back(llvm::ConstantArray::get(rowType, llvmBoolValues));
            }

            llvm::Constant* matrixConst = llvm::ConstantArray::get(matrixType, llvmRows);

            llvm::GlobalVariable* globalMatrix = new llvm::GlobalVariable(*module, matrixType, true, llvm::GlobalValue::PrivateLinkage, matrixConst, "booleanMatrixArray");
            globalMatrix->setAlignment(llvm::Align(1));
            llvm::Value* globalMatrixPtr = builder.CreateBitCast(globalMatrix, llvm::PointerType::get(context, 0));
            return globalMatrixPtr;
        }
    }

    llvm::Value* ArrayInitializerNode::generateByteArray(std::shared_ptr<AstContext> astContext) {
        return nullptr;
    }

    llvm::Value* ArrayInitializerNode::generateShortArray(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Generate Short Array ............");
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if(singleDimArray) {
            const auto& shortVector = std::get<std::vector<short>>(*singleDimArray);
            llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt16Ty(context), shortVector.size());
            llvm::Constant* arrayConst = llvm::ConstantDataArray::get(context, llvm::ArrayRef<short>(shortVector));
            llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, "shortArray");
            globalArray->setAlignment(llvm::Align(2));

            llvm::Value* globalArrayPtr = builder.CreateBitCast(globalArray, llvm::PointerType::get(context, 0));
            return globalArrayPtr;
        } else {
            const auto& shortMatrixPtr = std::get<std::vector<std::vector<short>>>(*multiDimArray);
            size_t numRows = shortMatrixPtr.size();
            size_t numCols = shortMatrixPtr.front().size();
            llvm::ArrayType* rowType = llvm::ArrayType::get(llvm::Type::getInt16Ty(context), numCols);  // [numCols x i32]
            llvm::ArrayType* matrixType = llvm::ArrayType::get(rowType, numRows);  // [numRows x [numCols x i32]]

            std::vector<llvm::Constant*> rowConstants;
            for (const auto& row : shortMatrixPtr) {
                std::vector<llvm::Constant*> elements;
                for (short val : row) {
                    elements.push_back(llvm::ConstantInt::get(llvm::Type::getInt16Ty(context), val));
                }
                rowConstants.push_back(llvm::ConstantArray::get(rowType, elements));
            }
    
            llvm::Constant* matrixConst = llvm::ConstantArray::get(matrixType, rowConstants);
            llvm::GlobalVariable* globalMatrix = new llvm::GlobalVariable(*module, matrixType, true, llvm::GlobalValue::PrivateLinkage, matrixConst, "shortMatrixArray");
            globalMatrix->setAlignment(llvm::Align(2));
            llvm::Value* globalMatrixPtr = builder.CreateBitCast(globalMatrix, llvm::PointerType::get(context, 0));
            return globalMatrixPtr;
        }
    }

    llvm::Value* ArrayInitializerNode::generateIntegerArray(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Generate Integer Array ............");
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if (singleDimArray) {
            const auto& intArrayPtr = std::get<std::vector<int>>(*singleDimArray);
            llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt32Ty(context), intArrayPtr.size());
            llvm::Constant* arrayConst = llvm::ConstantDataArray::get(context, llvm::ArrayRef<int>(intArrayPtr));
            llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, "intArray");
            globalArray->setAlignment(llvm::Align(16));

            llvm::Value* globalArrayPtr = builder.CreateBitCast(globalArray, llvm::PointerType::get(context, 0));
            return globalArrayPtr;
        } else {
            const auto& intMatrixPtr = std::get<std::vector<std::vector<int>>>(*multiDimArray);
            size_t numRows = intMatrixPtr.size();
            size_t numCols = intMatrixPtr.front().size();
            llvm::ArrayType* rowType = llvm::ArrayType::get(llvm::Type::getInt32Ty(context), numCols);  // [numCols x i32]
            llvm::ArrayType* matrixType = llvm::ArrayType::get(rowType, numRows);  // [numRows x [numCols x i32]]

            llvm::SmallVector<llvm::Constant*, 16> flattenedMatrix;
            for (const auto& row : intMatrixPtr) {
                llvm::SmallVector<llvm::Constant*, 16> rowConstants;
                for (int val : row) {
                    rowConstants.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), val));
                }
                flattenedMatrix.push_back(llvm::ConstantArray::get(rowType, rowConstants));
            }
        
            llvm::Constant* matrixConst = llvm::ConstantArray::get(matrixType, flattenedMatrix);
            llvm::GlobalVariable* globalMatrix = new llvm::GlobalVariable(*module, matrixType, true, llvm::GlobalValue::PrivateLinkage, matrixConst, "intMatrixArray");
            globalMatrix->setAlignment(llvm::Align(16));
            llvm::Value* globalMatrixPtr = builder.CreateBitCast(globalMatrix, llvm::PointerType::get(context, 0));
            return globalMatrixPtr;
        }
    }

    llvm::Value* ArrayInitializerNode::generateLongArray(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Generate Long Array ............");
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if(singleDimArray) {
            const auto& longVector = std::get<std::vector<long>>(*singleDimArray);
            llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt64Ty(context), longVector.size());
            llvm::Constant* arrayConst = llvm::ConstantDataArray::get(context, llvm::ArrayRef<long>(longVector));
            llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, "longArray");
            globalArray->setAlignment(llvm::Align(8));

            llvm::Value* globalArrayPtr = builder.CreateBitCast(globalArray, llvm::PointerType::get(context, 0));
            return globalArrayPtr;
        } else {
            const auto& longMatrixPtr = std::get<std::vector<std::vector<long>>>(*multiDimArray);
            size_t numRows = longMatrixPtr.size();
            size_t numCols = longMatrixPtr.front().size();
            llvm::ArrayType* rowType = llvm::ArrayType::get(llvm::Type::getInt64Ty(context), numCols);  // [numCols x i32]
            llvm::ArrayType* matrixType = llvm::ArrayType::get(rowType, numRows);  // [numRows x [numCols x i32]]

            std::vector<llvm::Constant*> rowConstants;
            for (const auto& row : longMatrixPtr) {
                std::vector<llvm::Constant*> elements;
                for (long val : row) {
                    elements.push_back(llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), val));
                }
                rowConstants.push_back(llvm::ConstantArray::get(rowType, elements));
            }
    
            llvm::Constant* matrixConst = llvm::ConstantArray::get(matrixType, rowConstants);
            llvm::GlobalVariable* globalMatrix = new llvm::GlobalVariable(*module, matrixType, true, llvm::GlobalValue::PrivateLinkage, matrixConst, "longMatrixArray");
            globalMatrix->setAlignment(llvm::Align(8));
            llvm::Value* globalMatrixPtr = builder.CreateBitCast(globalMatrix, llvm::PointerType::get(context, 0));
            return globalMatrixPtr;
        }
    }

    llvm::Value* ArrayInitializerNode::generateFloatArray(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Generation Float Array ............");
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if (singleDimArray) {
            const auto& floatVector = std::get<std::vector<float>>(*singleDimArray);
            llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getFloatTy(context), floatVector.size());
            llvm::Constant* arrayConst = llvm::ConstantDataArray::get(context, llvm::ArrayRef<float>(floatVector));
            llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, "floatArray");
            globalArray->setAlignment(llvm::Align(16));

            llvm::Value* globalArrayPtr = builder.CreateBitCast(globalArray, llvm::PointerType::get(context, 0));
            return globalArrayPtr;
        } else {
            const auto& floatMatrixPtr = std::get<std::vector<std::vector<float>>>(*multiDimArray);
            size_t numRows = floatMatrixPtr.size();
            size_t numCols = floatMatrixPtr.front().size();
            llvm::ArrayType* rowType = llvm::ArrayType::get(llvm::Type::getFloatTy(context), numCols);  // [numCols x i32]
            llvm::ArrayType* matrixType = llvm::ArrayType::get(rowType, numRows);  // [numRows x [numCols x i32]]

            std::vector<llvm::Constant*> rowConstants;
            for (const auto& row : floatMatrixPtr) {
                std::vector<llvm::Constant*> elements;
                for (float val : row) {
                    elements.push_back(llvm::ConstantFP::get(llvm::Type::getFloatTy(context), val));
                }
                rowConstants.push_back(llvm::ConstantArray::get(rowType, elements));
            }

            llvm::Constant* matrixConst = llvm::ConstantArray::get(matrixType, rowConstants);
            llvm::GlobalVariable* globalMatrix = new llvm::GlobalVariable(*module, matrixType, true, llvm::GlobalValue::PrivateLinkage, matrixConst, "floatMatrixArray");
            globalMatrix->setAlignment(llvm::Align(16));
            llvm::Value* globalMatrixPtr = builder.CreateBitCast(globalMatrix, llvm::PointerType::get(context, 0));
            return globalMatrixPtr;
        }
    }

    llvm::Value* ArrayInitializerNode::generateDoubleArray(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Generation Double Array ............");
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if (singleDimArray) {
            const auto& doubleVector = std::get<std::vector<double>>(*singleDimArray);
            llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getDoubleTy(context), doubleVector.size());
            llvm::Constant* arrayConst = llvm::ConstantDataArray::get(context, llvm::ArrayRef<double>(doubleVector));
            llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, "doubleArray");
            globalArray->setAlignment(llvm::Align(16));

            llvm::Value* globalArrayPtr = builder.CreateBitCast(globalArray, llvm::PointerType::get(context, 0));
            return globalArrayPtr;
        } else {
            const auto& doubleMatrixPtr = std::get<std::vector<std::vector<double>>>(*multiDimArray);
            size_t numRows = doubleMatrixPtr.size();
            size_t numCols = doubleMatrixPtr.front().size();
            llvm::ArrayType* rowType = llvm::ArrayType::get(llvm::Type::getDoubleTy(context), numCols);  // [numCols x i32]
            llvm::ArrayType* matrixType = llvm::ArrayType::get(rowType, numRows);  // [numRows x [numCols x i32]]

            std::vector<llvm::Constant*> rowConstants;
            for (const auto& row : doubleMatrixPtr) {
                std::vector<llvm::Constant*> elements;
                for (double val : row) {
                    elements.push_back(llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), val));
                }
                rowConstants.push_back(llvm::ConstantArray::get(rowType, elements));
            }

            llvm::Constant* matrixConst = llvm::ConstantArray::get(matrixType, rowConstants);
            llvm::GlobalVariable* globalMatrix = new llvm::GlobalVariable(*module, matrixType, true, llvm::GlobalValue::PrivateLinkage, matrixConst, "doubleMatrixArray");
            globalMatrix->setAlignment(llvm::Align(16));
            llvm::Value* globalMatrixPtr = builder.CreateBitCast(globalMatrix, llvm::PointerType::get(context, 0));
            return globalMatrixPtr;
        }

    }

    llvm::Value* ArrayInitializerNode::generateStringArray(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Generation String Array ............");
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if (singleDimArray) {
            const auto& stringVector = std::get<std::vector<std::string>>(*singleDimArray);
            std::vector<llvm::Constant*> stringPointers;
            for(const auto& str : stringVector) {
                llvm::StringRef strRef(str);
                llvm::Constant* strConst = llvm::ConstantDataArray::getString(context, strRef, true);
                stringPointers.push_back(strConst);    
            }

            llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), stringPointers.size());
            auto* arrayConst = llvm::ConstantArray::get(arrayType, stringPointers);

            llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, "stringArray");

            globalArray->setAlignment(llvm::Align(1));

            llvm::Value* globalArrayPtr = builder.CreateBitCast(globalArray, llvm::PointerType::get(context, 0));
            return globalArrayPtr;
        } else {
            const auto& stringMatrix = std::get<std::vector<std::vector<std::string>>>(*multiDimArray);
            size_t numRows = stringMatrix.size();
            size_t numCols = stringMatrix.front().size();
            llvm::ArrayType* rowType = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), numCols);
            llvm::ArrayType* matrixType = llvm::ArrayType::get(rowType, numRows);

            std::vector<llvm::Constant*> llvmRows;
            for (const auto& row : stringMatrix) {
                std::vector<llvm::Constant*> stringValues;
                for(const auto& str : row) {
                    llvm::StringRef strRef(str);
                    llvm::Constant* strConst = llvm::ConstantDataArray::getString(context, strRef, true);
                    stringValues.push_back(strConst);    
                }
                llvmRows.push_back(llvm::ConstantArray::get(rowType, stringValues));
            }

            llvm::Constant* matrixConst = llvm::ConstantArray::get(matrixType, llvmRows);
            llvm::GlobalVariable* globalMatrix = new llvm::GlobalVariable(*module, matrixType, true, llvm::GlobalValue::PrivateLinkage, matrixConst, "stringMatrixArray");
            globalMatrix->setAlignment(llvm::Align(1));
            llvm::Value* globalMatrixPtr = builder.CreateBitCast(globalMatrix, llvm::PointerType::get(context, 0));
            return globalMatrixPtr;
        }
    }


    llvm::Value* ArrayInitializerNode::generateCharArray(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Generation Character Array ............");
          auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();
        
        if (singleDimArray) {
            const auto& intArrayPtr = std::get<std::vector<char>>(*singleDimArray);
            llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), intArrayPtr.size());
            llvm::Constant* arrayConst = llvm::ConstantDataArray::get(context, llvm::ArrayRef<char>(intArrayPtr));
            llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, "charArray");
            globalArray->setAlignment(llvm::Align(1));

            llvm::Value* globalArrayPtr = builder.CreateBitCast(globalArray, llvm::PointerType::get(context, 0));
            return globalArrayPtr;
        } else {
            const auto& intMatrixPtr = std::get<std::vector<std::vector<char>>>(*multiDimArray);
            size_t numRows = intMatrixPtr.size();
            size_t numCols = intMatrixPtr.front().size();
            llvm::ArrayType* rowType = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), numCols);  // [numCols x i32]
            llvm::ArrayType* matrixType = llvm::ArrayType::get(rowType, numRows);  // [numRows x [numCols x i32]]

            llvm::SmallVector<llvm::Constant*, 1> flattenedMatrix;
            for (const auto& row : intMatrixPtr) {
                llvm::SmallVector<llvm::Constant*, 16> rowConstants;
                for (int val : row) {
                    rowConstants.push_back(llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), val));
                }
                flattenedMatrix.push_back(llvm::ConstantArray::get(rowType, rowConstants));
            }
        
            llvm::Constant* matrixConst = llvm::ConstantArray::get(matrixType, flattenedMatrix);
            llvm::GlobalVariable* globalMatrix = new llvm::GlobalVariable(*module, matrixType, true, llvm::GlobalValue::PrivateLinkage, matrixConst, "charMatrixArray");
            globalMatrix->setAlignment(llvm::Align(1));
            llvm::Value* globalMatrixPtr = builder.CreateBitCast(globalMatrix, llvm::PointerType::get(context, 0));
            return globalMatrixPtr;
        }

    }
}
