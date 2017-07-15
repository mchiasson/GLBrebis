/*******************************************************************************
 * MIT License
 * 
 * Copyright (c) 2017 Mathieu-André Chiasson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
package com.glbrebis

import org.codehaus.groovy.runtime.typehandling.GroovyCastException
import org.eclipse.cdt.core.dom.ast.ASTVisitor
import org.eclipse.cdt.core.dom.ast.IASTArrayDeclarator
import org.eclipse.cdt.core.dom.ast.IASTArrayModifier
import org.eclipse.cdt.core.dom.ast.IASTCompositeTypeSpecifier
import org.eclipse.cdt.core.dom.ast.IASTDeclSpecifier
import org.eclipse.cdt.core.dom.ast.IASTDeclaration
import org.eclipse.cdt.core.dom.ast.IASTDeclarator
import org.eclipse.cdt.core.dom.ast.IASTElaboratedTypeSpecifier
import org.eclipse.cdt.core.dom.ast.IASTFunctionDeclarator
import org.eclipse.cdt.core.dom.ast.IASTParameterDeclaration
import org.eclipse.cdt.core.dom.ast.IASTPointerOperator
import org.eclipse.cdt.core.dom.ast.IASTProblem
import org.eclipse.cdt.core.dom.ast.IASTSimpleDeclaration
import org.eclipse.cdt.core.dom.ast.IASTTranslationUnit
import org.eclipse.cdt.internal.core.dom.parser.cpp.CPPASTLinkageSpecification

class GLBrebisASTVisitor extends ASTVisitor
{
    Deque<GLBrebisGroup> m_groups = null

    Deque<GLBrebisDeclaration> m_currentDeclarations = null
    Deque<GLBrebisFunction> m_currentFunctions = null

    boolean m_bIsParameter = false
    boolean m_bIsTypedef = false
    boolean m_bIsStruct = false

    GLBrebisASTVisitor(Deque<GLBrebisGroup> groups)
    {
        m_groups = groups
        m_bIsParameter = false
    }

    GLBrebisGroup getGroup(String fileName, int lineNumer)
    {
        for (group in m_groups)
        {
            if (lineNumer < group.startLineNumber)
            {
                // outside of this group (too early in the file)
                continue
            }
            if (lineNumer > group.endLineNumber)
            {
                // outside of this group (too far in the file)
                continue
            }

            return group
        }
        return null
    }

    private static String transformSignature(String signature)
    {
        // Also removing/replacing some macro definition that we don't need...
        return GLBrebisASTParser.standardize(signature)
                .replace("EGLAPIENTRYP ", "*")
                .replace("EGLAPIENTRY ", "")
                .replace("EGLAPI", "")
                .replace("GL_APIENTRYP ", "*")
                .replace("GL_APIENTRY ", "")
                .replace("GL_APICALL", "")
                .replace("APIENTRYP ", "*")
                .replace("APIENTRY ", "")
                .replace("GLAPI", "")
                .replace("GL_API", "")
                .replace("( *", "(*")
    }

    @Override
    int visit(IASTDeclaration declaration)
    {
        if (declaration instanceof IASTSimpleDeclaration)
        {
            // For structs, we get IASTSimpleDeclaration for each members, so
            // we can simply skip those since we don't care about those.
            if (m_bIsStruct)
            {
                return PROCESS_SKIP
            }

            String [] path = declaration.getFileLocation().getFileName().split("/")

            GLBrebisGroup group = getGroup(path[path.length - 2] + "/" + path[path.length -1], declaration.getFileLocation().getEndingLineNumber())
            if (group != null)
            {
                if(declaration.getRawSignature().startsWith("struct") || declaration.getRawSignature().startsWith("typedef struct") || declaration.getRawSignature().startsWith("DECLARE_HANDLE"))
                {
                    m_bIsStruct = true
                    m_currentDeclarations = group.declarations
                    m_currentDeclarations.add(new GLBrebisDeclaration())
                    m_currentDeclarations.getLast().group = group
                    m_currentDeclarations.getLast().type = GLBrebisDeclaration.Type.STRUCT
                    m_currentDeclarations.getLast().signature = transformSignature(declaration.getRawSignature())
                    m_currentDeclarations.getLast().lineNumber = declaration.getFileLocation().getEndingLineNumber()
                }
                else if (!m_bIsTypedef && declaration.getRawSignature().matches("^typedef.*"))
                {
                    if (declaration.getRawSignature().matches(".*?\\bPFN.*?PROC\\b.*?"))
                    {
                        // we're not interested in any PFN*PROC function
                        // pointers since we can created them all from the
                        // function prototypes ourselves. Plus, sometimes, these
                        // function pointers are missing (see EGL/egl.h), so
                        // they're not a good reliable source of data.
                        return PROCESS_SKIP
                    }
                    m_bIsTypedef = true
                    m_currentDeclarations = group.declarations
                    m_currentDeclarations.add(new GLBrebisDeclaration())
                    m_currentDeclarations.getLast().group = group
                    m_currentDeclarations.getLast().type = GLBrebisDeclaration.Type.TYPEDEF
                    m_currentDeclarations.getLast().signature = transformSignature(declaration.getRawSignature())
                    m_currentDeclarations.getLast().lineNumber = declaration.getFileLocation().getEndingLineNumber()
                }
                else
                {
                    m_currentFunctions = group.functions
                    m_currentFunctions.add(new GLBrebisFunction())
                    m_currentFunctions.getLast().group = group
                    m_currentFunctions.getLast().lineNumber = declaration.getFileLocation().getEndingLineNumber()
                }
            }
            else
            {
                System.err.println("No group found for " + declaration.getRawSignature())
            }
        }

        return PROCESS_CONTINUE
    }

    @Override
    int leave(IASTDeclaration declaration)
    {
        if (m_bIsTypedef)
        {
            m_bIsTypedef = false
        }
        if (m_bIsStruct)
        {
            m_bIsStruct = false
        }
        else if (m_currentFunctions)
        {
            if (m_currentFunctions.getLast().name.isEmpty())
            {
                // I guess the current declaration wasn't a function after all...
                m_currentFunctions.removeLast()
            }
            else
            {
                m_currentFunctions.getLast().paramDeclaration = transformSignature(m_currentFunctions.getLast().paramDeclaration)
                m_currentFunctions.getLast().returnParam = transformSignature(m_currentFunctions.getLast().returnParam)
            }
        }

        return PROCESS_CONTINUE
    }

    @Override
    int visit(IASTDeclSpecifier specifier)
    {
        if (m_bIsParameter)
        {
            m_currentFunctions.getLast().paramDeclaration += " " + specifier.toString()
            m_currentFunctions.getLast().paramDeclaration = m_currentFunctions.getLast().paramDeclaration.trim()
        }
        else if (m_bIsTypedef)
        {
            m_currentDeclarations.getLast().value = transformSignature(specifier.toString())
        }
        else if (m_bIsStruct)
        {
            if (specifier instanceof IASTCompositeTypeSpecifier)
            {
                IASTCompositeTypeSpecifier composite = (IASTCompositeTypeSpecifier)specifier
                m_currentDeclarations.getLast().name = composite.getName().toString()
            }
            else if (specifier instanceof IASTElaboratedTypeSpecifier)
            {
                IASTElaboratedTypeSpecifier elaborated = (IASTElaboratedTypeSpecifier)specifier
                m_currentDeclarations.getLast().name = elaborated.getName().toString()
            }
        }
        else
        {
            // it's has to be the return specifier's IASTName node
            m_currentFunctions.getLast().returnParam += " " + specifier.toString()
            m_currentFunctions.getLast().returnParam = m_currentFunctions.getLast().returnParam.trim()
        }
        return PROCESS_CONTINUE
    }

    @Override
    int visit(IASTDeclarator declarator)
    {
        if (declarator instanceof IASTFunctionDeclarator)
        {
            if (m_bIsTypedef)
            {
                m_currentDeclarations.getLast().type = GLBrebisDeclaration.Type.TYPEDEF_FUNCPTR
            }
            else if (m_bIsStruct)
            {
                System.err.println("nothing to do, just absorb.")
            }
            else
            {
                // got the function's name!
                m_currentFunctions.getLast().name = declarator.getName().toString()

                // for example, 'glClear' turns to 'PFNGLCLEARPROC', which is Khronos function pointer naming convention.
                m_currentFunctions.getLast().pointerName = "PFN" + declarator.getName().toString().toUpperCase() + "PROC"

                // let's check if it has an IASTPointer Node
                for (pointerOp in declarator.getPointerOperators())
                {
                    // The pointer goes to the return token though, not the function name.
                    m_currentFunctions.getLast().returnParam += " " + pointerOp.getRawSignature()
                    m_currentFunctions.getLast().returnParam = m_currentFunctions.getLast().returnParam.trim()
                }
            }
        }
        else if (m_bIsTypedef)
        {
            // name can only be set once because function pointer parameters can
            // also be declarators. The first time the name will be set will be
            // (hopefully) the name of the function pointer. According to the
            // debugger, it looks like it is working.
            if (m_currentDeclarations.getLast().name.isEmpty())
            {
                m_currentDeclarations.getLast().name = declarator.getName().toString()
            }
        }
        else if (m_bIsStruct)
        {
            m_currentDeclarations.getLast().name = declarator.getName().toString()
        }
        else if (m_bIsParameter)
        {
            // let's check if it has an IASTPointer Node
            for (pointerOp in declarator.getPointerOperators())
            {
                // The pointer goes only to the function declaration
                m_currentFunctions.getLast().paramDeclaration += " " + pointerOp.getRawSignature()
                m_currentFunctions.getLast().paramDeclaration = m_currentFunctions.getLast().paramDeclaration.trim()
            }

            String varName = GLBrebisASTParser.standardize(declarator.getName().toString().toString())

            // both parameter declaration and call strings get the parameter name
            m_currentFunctions.getLast().paramDeclaration += " " + varName
            m_currentFunctions.getLast().paramDeclaration = m_currentFunctions.getLast().paramDeclaration.trim()
            m_currentFunctions.getLast().paramDeclaration = m_currentFunctions.getLast().paramDeclaration.replace("* ", "*")
            m_currentFunctions.getLast().paramCall += " " + varName
            m_currentFunctions.getLast().paramCall = m_currentFunctions.getLast().paramCall.trim()

            // Careful! sometimes, these parameters are arrays!
            if (declarator instanceof IASTArrayDeclarator)
            {
                IASTArrayDeclarator arrayDeclarator = (IASTArrayDeclarator)declarator
                for(arrayModifier in arrayDeclarator.getArrayModifiers())
                {
                    // it's has to be the return specifier's IASTName node
                    m_currentFunctions.getLast().paramDeclaration += arrayModifier.getRawSignature()
                    m_currentFunctions.getLast().paramDeclaration = m_currentFunctions.getLast().paramDeclaration.trim()
                }
            }
        }
        else
        {
            try
            {
                m_currentFunctions.getLast().returnParam += " " + declarator.getName().toString().toString()
                m_currentFunctions.getLast().returnParam = m_currentFunctions.getLast().returnParam.trim()
            }
            catch (Exception e)
            {
                System.err.println(declarator.getName().toString().toString())
            }
        }
        return PROCESS_CONTINUE
    }

    @Override
    int visit(IASTParameterDeclaration parameterDeclaration)
    {
        if (!m_bIsTypedef && !m_bIsStruct)
        {
            m_bIsParameter = true

            // only need to add a comma with this guy, the rest will be populated by the IASTPointer and IASTName nodes
            if (!m_currentFunctions.getLast().paramDeclaration.isEmpty())
            {
                m_currentFunctions.getLast().paramDeclaration += ","
            }

            if (!m_currentFunctions.getLast().paramCall.isEmpty())
            {
                m_currentFunctions.getLast().paramCall += ","
            }
        }

        return PROCESS_CONTINUE
    }

    @Override
    int leave(IASTParameterDeclaration parameterDeclaration)
    {
        if (!m_bIsTypedef && !m_bIsStruct)
        {
            m_bIsParameter = false
        }
        return PROCESS_CONTINUE
    }
    
    @Override
    int visit(IASTProblem problem)
    {
        throw new Exception("PARSING ERROR : Cannot parse '" + problem.getRawSignature() + "'")
    }
}