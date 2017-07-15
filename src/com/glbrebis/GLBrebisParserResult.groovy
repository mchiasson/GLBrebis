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


import java.util.Deque
import java.util.List


class GLBrebisParserResult
{
    public Deque<GLBrebisFile> files = new ArrayDeque<GLBrebisFile>()

    /*
     *  returns the first occurrence group with a name.
     */
    public GLBrebisGroup getGroup(groupName)
    {
        for (file in files)
        {
            for(group in file.groups)
            {
                if (group.name.equals(groupName))
                {
                    return group
                }
            }
        }
        return null
    }

    /*
     *  returns the first occurrence of a define.
     */
    public GLBrebisDeclaration getDefine(String defineName)
    {
        for (file in files)
        {
            for(group in file.groups)
            {
                for(declaration in group.declarations)
                {
                    if (declaration.type == GLBrebisDeclaration.Type.DEFINE && declaration.name.equals(defineName))
                    {
                        return declaration
                    }
                }
            }
        }
        return null
    }

    /*
     *  returns the first occurrence of a typedef.
     */
    public GLBrebisDeclaration getTypedef(String typedefName)
    {
        for (file in files)
        {
            for(group in file.groups)
            {
                for(declaration in group.declarations)
                {
                    if (declaration.type == GLBrebisDeclaration.Type.TYPEDEF && declaration.name.equals(typedefName))
                    {
                        return declaration
                    }
                }
            }
        }
        return null
    }

    /*
     *  returns the first occurrence of function pointer typedef.
     */
    public GLBrebisDeclaration getFuncPtr(String funcPtrName)
    {
        for (file in files)
        {
            for(group in file.groups)
            {
                for(declaration in group.declarations)
                {
                    if (declaration.type == GLBrebisDeclaration.Type.TYPEDEF_FUNCPTR && declaration.name.equals(funcPtrName))
                    {
                        return declaration
                    }
                }
            }
        }
        return null
    }

    /*
     *  returns the first occurrence of a struct.
     */
    public GLBrebisDeclaration getStruct(String structName)
    {
        for (file in files)
        {
            for(group in file.groups)
            {
                for(declaration in group.declarations)
                {
                    if (declaration.type == GLBrebisDeclaration.Type.STRUCT && declaration.name.equals(structName))
                    {
                        return declaration
                    }
                }
            }
        }
        return null
    }

    /*
     *  returns the first occurrence of a function with a name.
     */
    public GLBrebisFunction getFunction(functionName)
    {
        for (file in files)
        {
            for(group in file.groups)
            {
                for(function in group.functions)
                {
                    if (function.name.equals(functionName))
                    {
                        return function
                    }
                }
            }
        }
        return null
    }

    /*
     *  returns the a list of all the available version groups. Duplicates
     *  will automatically be excluded and the results will be sorted
     *  alphabetically. 
     */
    public List<GLBrebisGroup> getAllUniqueVersionGroups()
    {
        if (allUniqueVersionGroups == null)
        {
            allUniqueVersionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    if (group.isVersion() && !allUniqueVersionGroups.contains(group))
                    {
                        allUniqueVersionGroups.add(group)
                    }
                }
            }
            Collections.sort(allUniqueVersionGroups)
        }
        return allUniqueVersionGroups
    }
    private List<GLBrebisGroup> allUniqueVersionGroups = null

    /*
     *  returns the a list of all the available GLES version groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueGLESVersionGroups()
    {
        if (allUniqueGLESVersionGroups == null)
        {
            allUniqueGLESVersionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isGLES())
                {
                    for(group in file.groups)
                    {
                        if (group.isVersion() && !allUniqueGLESVersionGroups.contains(group))
                        {
                            allUniqueGLESVersionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLESVersionGroups)
        }
        return allUniqueGLESVersionGroups
    }
    private List<GLBrebisGroup> allUniqueGLESVersionGroups = null

    /*
     *  returns the a list of all the available GL version groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueGLVersionGroups()
    {
        if (allUniqueGLVersionGroups == null)
        {
            allUniqueGLVersionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isVersion() && !allUniqueGLVersionGroups.contains(group))
                        {
                            allUniqueGLVersionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLVersionGroups)
        }
        return allUniqueGLVersionGroups
    }
    private List<GLBrebisGroup> allUniqueGLVersionGroups = null

    /*
     *  returns the a list of all the available EGL version groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueEGLVersionGroups()
    {
        if (allUniqueEGLVersionGroups == null)
        {
            allUniqueEGLVersionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isEGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isVersion() && !allUniqueEGLVersionGroups.contains(group))
                        {
                            allUniqueEGLVersionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueEGLVersionGroups)
        }
        return allUniqueEGLVersionGroups
    }
    private List<GLBrebisGroup> allUniqueEGLVersionGroups = null


    /*
     *  returns the a list of all the available GLX version groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueGLXVersionGroups()
    {
        if (allUniqueGLXVersionGroups == null)
        {
            allUniqueGLXVersionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isGLX())
                {
                    for(group in file.groups)
                    {
                        if (group.isVersion() && !allUniqueGLXVersionGroups.contains(group))
                        {
                            allUniqueGLXVersionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLXVersionGroups)
        }
        return allUniqueGLXVersionGroups
    }
    private List<GLBrebisGroup> allUniqueGLXVersionGroups = null

    /*
     *  returns the a list of all the available WGL version groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueWGLVersionGroups()
    {
        if (allUniqueWGLVersionGroups == null)
        {
            allUniqueWGLVersionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isWGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isVersion() && !allUniqueWGLVersionGroups.contains(group))
                        {
                            allUniqueWGLVersionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueWGLVersionGroups)
        }
        return allUniqueWGLVersionGroups
    }
    private List<GLBrebisGroup> allUniqueWGLVersionGroups = null

    /*
     *  returns the a list of all the available extension groups. Duplicates
     *  will automatically be excluded and the results will be sorted
     *  alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueExtensionGroups()
    {
        if (allUniqueExtensionGroups == null)
        {
            allUniqueExtensionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    if (group.isExtension() && !allUniqueExtensionGroups.contains(group))
                    {
                        allUniqueExtensionGroups.add(group)
                    }
                }
            }
            Collections.sort(allUniqueExtensionGroups)
        }
        return allUniqueExtensionGroups
    }
    private List<GLBrebisGroup> allUniqueExtensionGroups = null

    /*
     *  returns the a list of all the available GLES extension groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueGLESExtensionGroups()
    {
        if (allUniqueGLESExtensionGroups == null)
        {
            allUniqueGLESExtensionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isGLES())
                {
                    for(group in file.groups)
                    {
                        if (group.isExtension() && !allUniqueGLESExtensionGroups.contains(group))
                        {
                            allUniqueGLESExtensionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLESExtensionGroups)
        }
        return allUniqueGLESExtensionGroups
    }
    private List<GLBrebisGroup> allUniqueGLESExtensionGroups = null

    /*
     *  returns the a list of all the available GL extension groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueGLExtensionGroups()
    {
        if (allUniqueGLExtensionGroups == null)
        {
            allUniqueGLExtensionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isExtension() && !allUniqueGLExtensionGroups.contains(group))
                        {
                            allUniqueGLExtensionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLExtensionGroups)
        }
        return allUniqueGLExtensionGroups
    }
    private List<GLBrebisGroup> allUniqueGLExtensionGroups = null

    /*
     *  returns the a list of all the available EGL extension groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueEGLExtensionGroups()
    {
        if (allUniqueEGLExtensionGroups == null)
        {
            allUniqueEGLExtensionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isEGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isExtension() && !allUniqueEGLExtensionGroups.contains(group))
                        {
                            allUniqueEGLExtensionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueEGLExtensionGroups)
        }
        return allUniqueEGLExtensionGroups
    }
    private List<GLBrebisGroup> allUniqueEGLExtensionGroups = null

    /*
     *  returns the a list of all the available GLX extension groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueGLXExtensionGroups()
    {
        if (allUniqueGLXExtensionGroups == null)
        {
            allUniqueGLXExtensionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isGLX())
                {
                    for(group in file.groups)
                    {
                        if (group.isExtension() && !allUniqueGLXExtensionGroups.contains(group))
                        {
                            allUniqueGLXExtensionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLXExtensionGroups)
        }
        return allUniqueGLXExtensionGroups
    }
    private List<GLBrebisGroup> allUniqueGLXExtensionGroups = null

    /*
     *  returns the a list of all the available WGL extension groups.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisGroup> getAllUniqueWGLExtensionGroups()
    {
        if (allUniqueWGLExtensionGroups == null)
        {
            allUniqueWGLExtensionGroups = new ArrayList<GLBrebisGroup>()
            for (file in files)
            {
                if (file.isWGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isExtension() && !allUniqueWGLExtensionGroups.contains(group))
                        {
                            allUniqueWGLExtensionGroups.add(group)
                        }
                    }
                }
            }
            Collections.sort(allUniqueWGLExtensionGroups)
        }
        return allUniqueWGLExtensionGroups
    }
    private List<GLBrebisGroup> allUniqueWGLExtensionGroups = null

    /*
     *  returns the a list of all the available defines. Duplicates will
     *  automatically be excluded and the results will be sorted
     *  alphabetically. 
     */
    public List<GLBrebisDeclaration> getAllUniqueDefines()
    {
        if (allUniqueDefines == null)
        {
            allUniqueDefines = new ArrayList<GLBrebisDeclaration>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    for(declaration in group.declarations)
                    {
                        if (declaration.type == GLBrebisDeclaration.Type.DEFINE && !allUniqueDefines.contains(declaration))
                        {
                            allUniqueDefines.add(declaration)
                        }
                    }
                }
            }
            Collections.sort(allUniqueDefines)
        }
        return allUniqueDefines
    }
    private List<GLBrebisDeclaration> allUniqueDefines = null

    /*
     *  returns the a list of all the available typedefs. Duplicates will
     *  automatically be excluded and the results will not be sorted.
     */
    public List<GLBrebisDeclaration> getAllUniqueTypedefs()
    {
        if (allUniqueTypedefs == null)
        {
            allUniqueTypedefs = new ArrayList<GLBrebisDeclaration>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    for(declaration in group.declarations)
                    {
                        if (declaration.type == GLBrebisDeclaration.Type.TYPEDEF && !allUniqueTypedefs.contains(declaration))
                        {
                            allUniqueTypedefs.add(declaration)
                        }
                    }
                }
            }
        }
        return allUniqueTypedefs
    }
    private List<GLBrebisDeclaration> allUniqueTypedefs = null

    /*
     *  returns the a list of all the available function pointer typedefs.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisDeclaration> getAllUniqueFuncPtrs()
    {
        if (allUniqueFuncPtrs == null)
        {
            allUniqueFuncPtrs = new ArrayList<GLBrebisDeclaration>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    for(declaration in group.declarations)
                    {
                        if (declaration.type == GLBrebisDeclaration.Type.TYPEDEF_FUNCPTR && !allUniqueFuncPtrs.contains(declaration))
                        {
                            allUniqueFuncPtrs.add(declaration)
                        }
                    }
                }
            }
            Collections.sort(allUniqueFuncPtrs)
        }
        return allUniqueFuncPtrs
    }
    private List<GLBrebisDeclaration> allUniqueFuncPtrs = null

    /*
     *  returns the a list of all the available data structures. Duplicates will
     *  automatically be excluded and the results will not be sorted.
     */
    public List<GLBrebisDeclaration> getAllUniqueStructs()
    {
        if (allUniqueStructs == null)
        {
            allUniqueStructs = new ArrayList<GLBrebisDeclaration>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    for(declaration in group.declarations)
                    {
                        if (declaration.type == GLBrebisDeclaration.Type.STRUCT && !allUniqueStructs.contains(declaration))
                        {
                            allUniqueStructs.add(declaration)
                        }
                    }
                }
            }
        }
        return allUniqueStructs
    }
    private List<GLBrebisDeclaration> allUniqueStructs = null
    /*
     *  returns the a list of all the available functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueFunctions()
    {
        if (allUniqueFunctions == null)
        {
            allUniqueFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    for(function in group.functions)
                    {
                        if (!allUniqueFunctions.contains(function))
                        {
                            allUniqueFunctions.add(function)
                        }
                    }
                }
            }
            Collections.sort(allUniqueFunctions)
        }
        return allUniqueFunctions
    }
    private List<GLBrebisFunction> allUniqueFunctions = null

    /*
     *  returns the a list of all the available GLES functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLESFunctions()
    {
        if (allUniqueGLESFunctions == null)
        {
            allUniqueGLESFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isGLES())
                {
                    for(group in file.groups)
                    {
                        for(function in group.functions)
                        {
                            if (!allUniqueGLESFunctions.contains(function))
                            {
                                allUniqueGLESFunctions.add(function)
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLESFunctions)
        }
        return allUniqueGLESFunctions
    }
    private List<GLBrebisFunction> allUniqueGLESFunctions = null

    /*
     *  returns the a list of all the available GL functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLFunctions()
    {
        if (allUniqueGLFunctions == null)
        {
            allUniqueGLFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isGL())
                {
                    for(group in file.groups)
                    {
                        for(function in group.functions)
                        {
                            if (!allUniqueGLFunctions.contains(function))
                            {
                                allUniqueGLFunctions.add(function)
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLFunctions)
        }
        return allUniqueGLFunctions
    }
    private List<GLBrebisFunction> allUniqueGLFunctions = null

    /*
     *  returns the a list of all the available EGL functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueEGLFunctions()
    {
        if (allUniqueEGLFunctions == null)
        {
            allUniqueEGLFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isEGL())
                {
                    for(group in file.groups)
                    {
                        for(function in group.functions)
                        {
                            if (!allUniqueEGLFunctions.contains(function))
                            {
                                allUniqueEGLFunctions.add(function)
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueEGLFunctions)
        }
        return allUniqueEGLFunctions
    }
    private List<GLBrebisFunction> allUniqueEGLFunctions = null

    /*
     *  returns the a list of all the available version functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueVersionFunctions()
    {
        if (allUniqueVersionFunctions == null)
        {
            allUniqueVersionFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    if (group.isVersion())
                    {
                        for(function in group.functions)
                        {
                            if (!allUniqueVersionFunctions.contains(function))
                            {
                                allUniqueVersionFunctions.add(function)
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueVersionFunctions)
        }
        return allUniqueVersionFunctions
    }
    private List<GLBrebisFunction> allUniqueVersionFunctions = null

    /*
     *  returns the a list of all the available GLES version functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLESVersionFunctions()
    {
        if (allUniqueGLESVersionFunctions == null)
        {
            allUniqueGLESVersionFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isGLES())
                {
                    for(group in file.groups)
                    {
                        if (group.isVersion())
                        {
                            for(function in group.functions)
                            {
                                if (!allUniqueGLESVersionFunctions.contains(function))
                                {
                                    allUniqueGLESVersionFunctions.add(function)
                                }
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLESVersionFunctions)
        }
        return allUniqueGLESVersionFunctions
    }
    private List<GLBrebisFunction> allUniqueGLESVersionFunctions = null

    /*
     *  returns the a list of all the available GL version functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLVersionFunctions()
    {
        if (allUniqueGLVersionFunctions == null)
        {
            allUniqueGLVersionFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isVersion())
                        {
                            for(function in group.functions)
                            {
                                if (!allUniqueGLVersionFunctions.contains(function))
                                {
                                    allUniqueGLVersionFunctions.add(function)
                                }
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLVersionFunctions)
        }
        return allUniqueGLVersionFunctions
    }
    private List<GLBrebisFunction> allUniqueGLVersionFunctions = null

    /*
     *  returns the a list of all the available EGL version functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueEGLVersionFunctions()
    {
        if (allUniqueEGLVersionFunctions == null)
        {
            allUniqueEGLVersionFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isEGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isVersion())
                        {
                            for(function in group.functions)
                            {
                                if (!allUniqueEGLVersionFunctions.contains(function))
                                {
                                    allUniqueEGLVersionFunctions.add(function)
                                }
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueEGLVersionFunctions)
        }
        return allUniqueEGLVersionFunctions
    }
    private List<GLBrebisFunction> allUniqueEGLVersionFunctions = null

    /*
     *  returns the a list of all the available version functions from a
     *  given version group. Duplicates will automatically be excluded and
     *  the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueFunctionsFromVersion(String versionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            for(group in file.groups)
            {
                if (group.isVersion() && group.name.equals(versionName))
                {
                    for(function in group.functions)
                    {
                        if (!results.contains(function))
                        {
                            results.add(function)
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available GLES version functions from
     *  a given version group. Duplicates will automatically be excluded and
     *  the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLESFunctionsFromVersion(String versionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGLES())
            {
                for(group in file.groups)
                {
                    if (group.isVersion() && group.name.equals(versionName))
                    {
                        for(function in group.functions)
                        {
                            if (!results.contains(function))
                            {
                                results.add(function)
                            }
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available GL version functions from
     *  a given version group. Duplicates will automatically be excluded and
     *  the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLFunctionsFromVersion(String versionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGL())
            {
                for(group in file.groups)
                {
                    if (group.isVersion() && group.name.equals(versionName))
                    {
                        for(function in group.functions)
                        {
                            if (!results.contains(function))
                            {
                                results.add(function)
                            }
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available EGL version functions from
     *  a given version group. Duplicates will automatically be excluded and
     *  the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueEGLFunctionsFromVersion(String versionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isEGL())
            {
                for(group in file.groups)
                {
                    if (group.isVersion() && group.name.equals(versionName))
                    {
                        for(function in group.functions)
                        {
                            if (!results.contains(function))
                            {
                                results.add(function)
                            }
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available extension functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueExtensionFunctions()
    {
        if (allUniqueExtensionFunctions == null)
        {
            allUniqueExtensionFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                for(group in file.groups)
                {
                    if (group.isExtension())
                    {
                        for(function in group.functions)
                        {
                            if (!allUniqueExtensionFunctions.contains(function) && !getAllUniqueVersionFunctions().contains(function))
                            {
                                allUniqueExtensionFunctions.add(function)
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueExtensionFunctions)
        }
        return allUniqueExtensionFunctions
    }
    private List<GLBrebisFunction> allUniqueExtensionFunctions = null


    /*
     *  returns the a list of all the available GLES extension functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLESExtensionFunctions()
    {
        if (allUniqueGLESExtensionFunctions == null)
        {
            allUniqueGLESExtensionFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isGLES())
                {
                    for(group in file.groups)
                    {
                        if (group.isExtension())
                        {
                            for(function in group.functions)
                            {
                                if (!allUniqueGLESExtensionFunctions.contains(function))
                                {
                                    allUniqueGLESExtensionFunctions.add(function)
                                }
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLESExtensionFunctions)
        }
        return allUniqueGLESExtensionFunctions
    }
    private List<GLBrebisFunction> allUniqueGLESExtensionFunctions = null

    /*
     *  returns the a list of all the available GL extension functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLExtensionFunctions()
    {
        if (allUniqueGLExtensionFunctions == null)
        {
            allUniqueGLExtensionFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isExtension())
                        {
                            for(function in group.functions)
                            {
                                if (!allUniqueGLExtensionFunctions.contains(function))
                                {
                                    allUniqueGLExtensionFunctions.add(function)
                                }
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueGLExtensionFunctions)
        }
        return allUniqueGLExtensionFunctions
    }
    private List<GLBrebisFunction> allUniqueGLExtensionFunctions = null


    /*
     *  returns the a list of all the available EGL extension functions.
     *  Duplicates will automatically be excluded and the results will be
     *  sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueEGLExtensionFunctions()
    {
        if (allUniqueEGLExtensionFunctions == null)
        {
            allUniqueEGLExtensionFunctions = new ArrayList<GLBrebisFunction>()
            for (file in files)
            {
                if (file.isEGL())
                {
                    for(group in file.groups)
                    {
                        if (group.isExtension())
                        {
                            for(function in group.functions)
                            {
                                if (!allUniqueEGLExtensionFunctions.contains(function))
                                {
                                    allUniqueEGLExtensionFunctions.add(function)
                                }
                            }
                        }
                    }
                }
            }
            Collections.sort(allUniqueEGLExtensionFunctions)
        }
        return allUniqueEGLExtensionFunctions
    }
    private List<GLBrebisFunction> allUniqueEGLExtensionFunctions = null

    /*
     *  returns the a list of all the available extension functions from a
     *  given extension group. Duplicates will automatically be excluded and
     *  the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueFunctionsFromExtension(String extensionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            for(group in file.groups)
            {
                if (group.isExtension() && group.name.equals(extensionName))
                {
                    for(function in group.functions)
                    {
                        if (!results.contains(function))
                        {
                            results.add(function)
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available GLES extension functions
     *  from a given extension group. Duplicates will automatically be
     *  excluded and the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLESFunctionsFromExtension(String extensionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGLES())
            {
                for(group in file.groups)
                {
                    if (group.isExtension() && group.name.equals(extensionName))
                    {
                        for(function in group.functions)
                        {
                            if (!results.contains(function))
                            {
                                results.add(function)
                            }
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available GL extension functions
     *  from a given extension group. Duplicates will automatically be
     *  excluded and the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLFunctionsFromExtension(String extensionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGL())
            {
                for(group in file.groups)
                {
                    if (group.isExtension() && group.name.equals(extensionName))
                    {
                        for(function in group.functions)
                        {
                            if (!results.contains(function))
                            {
                                results.add(function)
                            }
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available EGL extension functions
     *  from a given extension group. Duplicates will automatically be
     *  excluded and the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueEGLFunctionsFromExtension(String extensionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isEGL())
            {
                for(group in file.groups)
                {
                    if (group.isExtension() && group.name.equals(extensionName))
                    {
                        for(function in group.functions)
                        {
                            if (!results.contains(function))
                            {
                                results.add(function)
                            }
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available GLX extension functions
     *  from a given extension group. Duplicates will automatically be
     *  excluded and the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueGLXFunctionsFromExtension(String extensionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGLX())
            {
                for(group in file.groups)
                {
                    if (group.isExtension() && group.name.equals(extensionName))
                    {
                        for(function in group.functions)
                        {
                            if (!results.contains(function))
                            {
                                results.add(function)
                            }
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the available WGL extension functions
     *  from a given extension group. Duplicates will automatically be
     *  excluded and the results will be sorted alphabetically.
     */
    public List<GLBrebisFunction> getAllUniqueWGLFunctionsFromExtension(String extensionName)
    {
        List<GLBrebisFunction> results = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isWGL())
            {
                for(group in file.groups)
                {
                    if (group.isExtension() && group.name.equals(extensionName))
                    {
                        for(function in group.functions)
                        {
                            if (!results.contains(function))
                            {
                                results.add(function)
                            }
                        }
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     *  returns the a list of all the groups that has a given function
     *  inside of it. All duplicates will be included, and the results
     *  will be sorted alphabetically. It is the users responsibility to
     *  manage the duplicated entry with the condition of their choice.
     *  
     *  We use this function for doxygen generation, where we want to know
     *  all of the origins of a given function.
     */
    public List<GLBrebisGroup> getAllDuplicatedGroupsWithFunction(String functionName)
    {
        List<GLBrebisGroup> results = new ArrayList<GLBrebisGroup>()

        // first we get all the non-unique functions including all duplicates
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            for(group in file.groups)
            {
                for(function in group.functions)
                {
                    if (function.name.equals(functionName))
                    {
                        results.add(group)
                    }
                }
            }
        }
        Collections.sort(results)
        return results
    }

    /*
     * Checks if a given version or extension is from GLES.
     * 
     * \note bare in mind that some extensions can be available in both
     *       GLES and GL. Do not assume that if this function returns true
     *       that it will be false with its GL counterpart.
     */
    public boolean isGLESGroup(String groupName)
    {
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGLES())
            {
                for(group in file.groups)
                {
                    if (group.name.equals(groupName))
                    {
                        return true
                    }
                }
            }
        }
        return false
    }

    /*
     * Checks if a given version or extension is from GL.
     *
     * \note bare in mind that some extensions can be available in both
     *       GLES and GL. Do not assume that if this function returns true
     *       that it will be false with its GLES counterpart.
     */
    public boolean isGLGroup(String groupName)
    {
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGL())
            {
                for(group in file.groups)
                {
                    if (group.name.equals(groupName))
                    {
                        return true
                    }
                }
            }
        }
        return false
    }

    /*
     * Checks if a given version or extension is from EGL.
     */
    public boolean isEGLGroup(String groupName)
    {
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isEGL())
            {
                for(group in file.groups)
                {
                    if (group.name.equals(groupName))
                    {
                        return true
                    }
                }
            }
        }
        return false
    }

    /*
     * Checks if a given function is from a version group.
     */
    public boolean isVersionFunction(String functionName)
    {
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            for(group in file.groups)
            {
                if (group.isVersion())
                {
                    for (function in group.functions)
                    {
                        if (function.name.equals(functionName))
                        {
                            return true
                        }
                    }
                }
            }
        }
        return false
    }

    /*
     * Checks if a given function is from an extension group.
     */
    public boolean isExtensionFunction(String functionName)
    {
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            for(group in file.groups)
            {
                if (group.isExtension())
                {
                    for (function in group.functions)
                    {
                        if (function.name.equals(functionName))
                        {
                            return true
                        }
                    }
                }
            }
        }
        return false
    }

    /*
     * Checks if a given function is from GLES.
     *
     * \note bare in mind that some extensions can be available in both
     *       GLES and GL. Do not assume that if this function returns true
     *       that it will be false with its GL counterpart.
     */
    public boolean isGLESFunction(String functionName)
    {
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGLES())
            {
                for(group in file.groups)
                {
                    for (function in group.functions)
                    {
                        if (function.name.equals(functionName))
                        {
                            return true
                        }
                    }
                }
            }
        }
        return false
    }

    /*
     * Checks if a given function is from GL.
     *
     * \note bare in mind that some extensions can be available in both
     *       GLES and GL. Do not assume that if this function returns true
     *       that it will be false with its GLES counterpart.
     */
    public boolean isGLFunction(String functionName)
    {
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isGL())
            {
                for(group in file.groups)
                {
                    for (function in group.functions)
                    {
                        if (function.name.equals(functionName))
                        {
                            return true
                        }
                    }
                }
            }
        }
        return false
    }

    /*
     * Checks if a given function is from EGL.
     */
    public boolean isEGLFunction(String functionName)
    {
        List<GLBrebisFunction> functions = new ArrayList<GLBrebisFunction>()
        for (file in files)
        {
            if (file.isEGL())
            {
                for(group in file.groups)
                {
                    for (function in group.functions)
                    {
                        if (function.name.equals(functionName))
                        {
                            return true
                        }
                    }
                }
            }
        }
        return false
    }
}
