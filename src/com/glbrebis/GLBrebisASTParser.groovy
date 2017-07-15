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


import java.util.ArrayList
import java.util.Deque
import java.util.List

import org.eclipse.cdt.core.dom.ast.ExpansionOverlapsBoundaryException
import org.eclipse.cdt.core.dom.ast.IASTDeclaration
import org.eclipse.cdt.core.dom.ast.IASTNode
import org.eclipse.cdt.core.dom.ast.IASTPreprocessorEndifStatement
import org.eclipse.cdt.core.dom.ast.IASTPreprocessorIfStatement
import org.eclipse.cdt.core.dom.ast.IASTPreprocessorIfdefStatement
import org.eclipse.cdt.core.dom.ast.IASTPreprocessorIfndefStatement
import org.eclipse.cdt.core.dom.ast.IASTPreprocessorMacroDefinition
import org.eclipse.cdt.core.dom.ast.IASTTranslationUnit
import org.eclipse.cdt.core.dom.ast.gnu.cpp.GPPLanguage
import org.eclipse.cdt.core.model.ILanguage
import org.eclipse.cdt.core.parser.DefaultLogService
import org.eclipse.cdt.core.parser.FileContent
import org.eclipse.cdt.core.parser.IParserLogService
import org.eclipse.cdt.core.parser.IScannerInfo
import org.eclipse.cdt.core.parser.IncludeFileContentProvider
import org.eclipse.cdt.core.parser.ScannerInfo
import org.eclipse.cdt.internal.core.dom.parser.cpp.CPPASTLinkageSpecification


class GLBrebisASTParser
{
    private static final String s_includePath = "downloads/"

    private GLBrebisParserResult m_result = new GLBrebisParserResult()

    public GLBrebisParserResult getResults()
    {
        return m_result
    }

    private void logASTTree(StringBuilder astLogBuilder, IASTNode node, int index)
    {
        IASTNode[] children = node.getChildren()

        boolean printContents = true

        String info = ""
        if (node instanceof IASTTranslationUnit)
        {
            info = " fileName=\"" + node.getFileLocation().getFileName() + "\""
            printContents = false
        }
        else if (node instanceof CPPASTLinkageSpecification)
        {
            CPPASTLinkageSpecification linkageSpec = (CPPASTLinkageSpecification)node
            info = " spec=\"" + linkageSpec.toString() + "\""
            printContents = false
        }
        else if (node instanceof IASTDeclaration)
        {
            info = " lineNumber=\"" + node.getFileLocation().getEndingLineNumber() + "\""
        }

        String tabs = ""
        for (int i = 0; i < index; ++i)
        {
            tabs += "  "
        }

        astLogBuilder.append(tabs + "<" + node.getClass().getSimpleName() + info + ">\n")

        if (printContents)
        {
            astLogBuilder.append(tabs + "  <RawSignature>" + node.getRawSignature() + "</RawSignature>\n")
        }

        for (iastNode in children)
        {
            logASTTree(astLogBuilder, iastNode, index + 1)
        }

        astLogBuilder.append(tabs + "</" + node.getClass().getSimpleName() + ">\n")
    }

    public static String standardize(String signature)
    {
        return signature
                .replaceAll("\\bkhronos_uint8_t\\b",             "uint8_t")
                .replaceAll("\\bkhronos_int8_t\\b",              "int8_t")
                .replaceAll("\\bkhronos_uint16_t\\b",            "uint16_t")
                .replaceAll("\\bkhronos_int16_t\\b",             "int16_t")
                .replaceAll("\\bkhronos_uint32_t\\b",            "uint32_t")
                .replaceAll("\\bkhronos_int32_t\\b",             "int32_t")
                .replaceAll("\\bkhronos_uint64_t\\b",            "uint64_t")
                .replaceAll("\\bkhronos_utime_nanoseconds_t\\b", "uint64_t")
                .replaceAll("\\bkhronos_int64_t\\b",             "int64_t")
                .replaceAll("\\bkhronos_stime_nanoseconds_t\\b", "int64_t")
                .replaceAll("\\bkhronos_float_t\\b",             "float")
                .replaceAll("\\bkhronos_double_t\\b",            "double")
                .replaceAll("\\bkhronos_uintptr_t\\b",           "uintptr_t")
                .replaceAll("\\bkhronos_intptr_t\\b",            "intptr_t")
                .replaceAll("\\bkhronos_usize_t\\b",             "size_t")
                .replaceAll("\\bkhronos_ssize_t\\b",             "intptr_t")
                .replaceAll("\\bunsigned\\s+?char\\b",           "uint8_t")
                .replaceAll("\\bsigned\\s+?char\\b",             "int8_t")
                .replaceAll("\\bunsigned\\s+?short\\s+?int\\b",  "uint16_t")
                .replaceAll("\\bunsigned\\s+?short\\b",          "uint16_t")
                .replaceAll("\\bsigned\\s+?short\\s+?int\\b",    "int16_t")
                .replaceAll("\\bsigned\\s+?short\\b",            "int16_t")
                .replaceAll("\\bshort\\s+?int\\b",               "int16_t")
                .replaceAll("\\bshort\\b",                       "int16_t")
                .replaceAll("\\bunsigned\\s+?int\\b",            "uint32_t")
                .replaceAll("\\bsigned\\s+?int\\b",              "int32_t")
                .replaceAll("\\bint\\b",                         "int32_t")
                .replaceAll("\\bssize_t\\b",                     "intptr_t")
                .replaceAll("\\busize_t\\b",                     "size_t")
    }


    public void parse(String url, String fileName, Boolean force)
    {
        GLBrebisUtilities.download(url, s_includePath + fileName, force)
        parse(fileName)
    }

    public void parse(String fileName)
    {
        String fileToParse = s_includePath + fileName

        System.out.println("Parsing '" + fileToParse + "' ...")

        FileContent fileContent = FileContent.createForExternalFileLocation(fileToParse)

        Map<String, String> definedSymbols = new HashMap<String, String>()

        // Essentially pretending that we do
        //    -DGL_GLEXT_PROTOTYPES=1
        //    -DGL_APIENTRY
        //    -DGL_APIENTRYP=*
        //    -DGL_APICALL
        //    -DEGL_EGLEXT_PROTOTYPES=1
        //    -DEGLAPIENTRY
        //    -DEGLAPIENTRYP=*
        //    -DEGLAPI
        //    -DKHRONOS_SUPPORT_INT64
        // We're doing this to help the parser 'see' everything, since
        // the code section between the false #ifdef/#ifndef and their
        // corresponding #endif would be disabled.
        definedSymbols.put("APIENTRY", "")
        definedSymbols.put("APIENTRYP", "*")
        definedSymbols.put("CONST", "const")
        definedSymbols.put("EGL_EGLEXT_PROTOTYPES", "1")
        definedSymbols.put("EGLAPIENTRY", "")
        definedSymbols.put("EGLAPIENTRYP", "*")
        definedSymbols.put("EGLAPI", "extern")
        definedSymbols.put("FAR", "")
        definedSymbols.put("GL_GLEXT_PROTOTYPES", "1")
        definedSymbols.put("GLAPI", "extern")
        definedSymbols.put("GL_API", "extern")
        definedSymbols.put("GL_APIENTRY", "")
        definedSymbols.put("GL_APIENTRYP", "*")
        definedSymbols.put("GL_APICALL", "extern")
        definedSymbols.put("GLX_GLXEXT_PROTOTYPES", "1")
        definedSymbols.put("KHRONOS_SUPPORT_INT64", "")
        definedSymbols.put("WGL_WGLEXT_PROTOTYPES", "1")
        definedSymbols.put("WINAPI", "")
        definedSymbols.put("WINGDIAPI", "")

        String[] includePaths = new String[1]
        includePaths[0] = s_includePath

        IScannerInfo info = new ScannerInfo(definedSymbols, includePaths)
        IParserLogService log = new DefaultLogService()

        IncludeFileContentProvider fileCreator = IncludeFileContentProvider.getEmptyFilesProvider()

        int opts = ILanguage.OPTION_IS_SOURCE_UNIT
        IASTTranslationUnit translationUnit = GPPLanguage.getDefault().getASTTranslationUnit(fileContent, info, fileCreator, null, opts, log)

        GLBrebisFile newFile = new GLBrebisFile(fileName)

        // Now we scan for all the groups from within our AST tree.
        // A group is defined as whatever we find sandwiched between macro
        // definition following this pattern:
        //
        //    #ifndef EGL_KHR_stream
        //    #define EGL_KHR_stream 1
        //    ...                           <----- Group content
        //    #endif /* EGL_KHR_stream */
        //
        // Inside of those groups, we will gather typedefs, defines and
        // function prototypes.
        //
        // Every Khronos headers follow this pattern. Inside groups, we
        // gather all the typedef, #defines and functions that we find in
        // them.
        extractGroupsFromASTTree(translationUnit, newFile)

        if (!newFile.groups.isEmpty())
        {
            //StringBuilder astLogBuilder = new StringBuilder()
            //logASTTree(astLogBuilder, translationUnit, 1)
            //GLBrebisUtilities.writeFile("logs/" + fileName.replace("/", "_") + ".log.xml", astLogBuilder.toString())

            GLBrebisASTVisitor visitor = new GLBrebisASTVisitor(newFile.groups)
            visitor.shouldVisitTranslationUnit = true
            visitor.shouldVisitDeclarations = true
            visitor.shouldVisitDeclSpecifiers = true
            visitor.shouldVisitDeclarators = true
            visitor.shouldVisitParameterDeclarations = true
            visitor.shouldVisitProblems = true

            translationUnit.accept(visitor)

            sanityCheck(newFile.groups)

            m_result.files.add(newFile)
        }
    }

    private void extractGroupsFromASTTree(IASTTranslationUnit translationUnit, GLBrebisFile newFile)
    {
        GLBrebisGroup currentGroup = null
        int nestedIfCount = 0
        for (statement in translationUnit.getAllPreprocessorStatements())
        {
            if (statement instanceof IASTPreprocessorIfStatement)
            {
                nestedIfCount++
            }
            else if( statement instanceof IASTPreprocessorIfdefStatement)
            {
                nestedIfCount++
            }
            else if (statement instanceof IASTPreprocessorIfndefStatement)
            {
                // we need to look for 1 here because the 1st nesting is from the header guard (example: '#ifndef __gl_h_')
                if (nestedIfCount == 1 && statement.getRawSignature().matches("^#ifndef E?W?GLX?_.+\$"))
                {
                    // found an exception in GLES3/gl32.h. This is not a real group...
                    if (!statement.getRawSignature().contains("GL_APIENTRYP"))
                    {
                        currentGroup = new GLBrebisGroup()
                        // first token is '#', second token is 'ifndef', third token is the group name, which is what we need.
                        currentGroup.name = statement.getSyntax().getNext().getNext().toString()
                        currentGroup.startLineNumber = statement.getFileLocation().getStartingLineNumber()
                        currentGroup.file = newFile

                        newFile.groups.add(currentGroup)
                    }
                }

                nestedIfCount++
            }
            else if (statement instanceof IASTPreprocessorEndifStatement)
            {
                nestedIfCount--

                // we need to look for 1 here because the 1st nesting is from the header guard (example: '#ifndef __gl_h_')
                if (nestedIfCount == 1 && currentGroup != null)
                {
                    currentGroup.endLineNumber = statement.getFileLocation().getStartingLineNumber()
                    currentGroup = null
                }
            }
            else if (statement instanceof IASTPreprocessorMacroDefinition)
            {
                if (currentGroup != null && statement.getRawSignature().matches("^#define\\s+E?W?GLX?_.+\$"))
                {
                    // We don't want this (for example) #define GL_VERSION_4_5 1
                    if (!statement.getRawSignature().contains(currentGroup.name))
                    {
                        GLBrebisDeclaration define = new GLBrebisDeclaration()

                        // first token is '#', second token is 'define', third token is the define name
                        define.name = statement.getSyntax().getNext().getNext().toString()
                        define.type = GLBrebisDeclaration.Type.DEFINE
                        define.signature = statement.getRawSignature()
                        define.group = currentGroup
                        define.lineNumber = statement.getFileLocation().getStartingLineNumber()
                        define.value = statement.getRawSignature().replace("#define", "").replace(define.name, "").trim()

                        currentGroup.declarations.add(define)
                    }
                }
            }
        }
    }

    /*
     * This loop is completely optional. This is only to discover any
     * problems such as mismatched defines, typedefs and function pointers,
     * which could lead to some very serious problems, and warn the user
     * if we find anything. It adds a good 30-45 seconds, but the output
     * is really worth it!
     */
    private sanityCheck(Deque<GLBrebisGroup> newGroupsFromParsedFile)
    {
        for (newGroup in newGroupsFromParsedFile)
        {
            for (newDeclaration in newGroup.declarations)
            {
                if (newDeclaration.type == GLBrebisDeclaration.Type.DEFINE)
                {
                    GLBrebisDeclaration oldDefine =  m_result.getDefine(newDeclaration.name)
                    if (oldDefine)
                    {
                        if (!newDeclaration.value.equals(oldDefine.value) && 
                            // We've addressed this critical issue in the code generator. 
                            // only function impacted was glGetProgramPipelineivEXT, where
                            // one of the expected parameter can be either GL_ACTIVE_SHADER
                            // or GL_CURRENT_SHADER depending if we're on GL or GLES.
                            newDeclaration.name != "GL_ACTIVE_PROGRAM_EXT" 
                            )
                        {
                            System.err.println("CRITICAL! Detected duplicated define '" + newDeclaration + "'")
                            System.err.println("\tfrom previously parsed '" + oldDefine + "'")
                            System.err.println("\tbut their signatures differ:")
                            System.err.println("\t\t" + newDeclaration.signature)
                            System.err.println("\tvs")
                            System.err.println("\t\t" + oldDefine.signature)
                        }
                    }
                }
                else if (newDeclaration.type == GLBrebisDeclaration.Type.TYPEDEF)
                {
                    GLBrebisDeclaration oldTypedef = m_result.getTypedef(newDeclaration.name)
                    if (oldTypedef)
                    {
                        if (!newDeclaration.value.equals(oldTypedef.value) && 
                            // We've checked this warning. GLint vs int32_t. 
                            // can be safely ignore.
                            newDeclaration.name != "GLfixed" && 
                            // We've checked this warning: ptrdiff_t vs intptr_t. 
                            // can be safely ignore.
                            newDeclaration.name != "GLintptr" &&
                            // We've checked this warning: ptrdiff_t vs intptr_t.
                            // can be safely ignore. 
                            newDeclaration.name != "GLsizeiptr" 
                            )
                        {
                            System.err.println("WARNING! Detected duplicated typedef '" + newDeclaration + "'")
                            System.err.println("\tfrom previously parsed '" + oldTypedef + "'")
                            System.err.println("\tbut their signatures differ:")
                            System.err.println("\t\t" + newDeclaration.signature)
                            System.err.println("\tvs")
                            System.err.println("\t\t" + oldTypedef.signature)
                        }
                    }
                }
                else if (newDeclaration.type == GLBrebisDeclaration.Type.TYPEDEF_FUNCPTR)
                {
                    GLBrebisDeclaration oldFuncPtr = m_result.getFuncPtr(newDeclaration.name)
                    if (oldFuncPtr)
                    {
                        if (!newDeclaration.value.equals(oldFuncPtr.value))
                        {
                            System.err.println("CRITICAL! Detected duplicated function pointer typedef '" + newDeclaration + "'")
                            System.err.println("\tfrom previously parsed '" + oldFuncPtr + "'")
                            System.err.println("\tbut their signatures differ:")
                            System.err.println("\t\t" + newDeclaration.signature)
                            System.err.println("\tvs")
                            System.err.println("\t\t" + oldFuncPtr.signature)
                        }
                    }
                }
                else if (newDeclaration.type == GLBrebisDeclaration.Type.STRUCT)
                {
                    GLBrebisDeclaration oldStruct =  m_result.getStruct(newDeclaration.name)
                    if (oldStruct)
                    {
                        if (!newDeclaration.signature.equals(oldStruct.signature))
                        {
                            System.err.println("CRITICAL! Detected duplicated struct '" + newDeclaration + "'")
                            System.err.println("\tfrom previously parsed '" + oldStruct + "'")
                            System.err.println("\tbut their signatures differ:")
                            System.err.println("\t\t" + newDeclaration.signature)
                            System.err.println("\tvs")
                            System.err.println("\t\t" + oldStruct.signature)
                        }
                    }
                }
            }
            for (newFunction in newGroup.functions)
            {
                GLBrebisFunction oldFunction = m_result.getFunction(newFunction.name)
                newFunction.signature = newFunction.returnParam + " " + newFunction.name + " (" + newFunction.paramDeclaration + ");"
                if (oldFunction)
                {
                    if (!newFunction.signature.equals(oldFunction.signature))
                    {
                        System.err.println("CRITICAL! Detected duplicated function '" + newFunction + "'")
                        System.err.println("\tfrom previously parsed '" + oldFunction + "'")
                        System.err.println("\tbut their signatures differ:")
                        System.err.println("\t\t" + newFunction.signature)
                        System.err.println("\tvs")
                        System.err.println("\t\t" + oldFunction.signature)
                    }
                }
            }
        }
    }
}

