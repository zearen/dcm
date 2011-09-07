" Vim syntax file
" Language: dcm
" Maintainer:   Zachary Weaver <zearen.wover@gmail.com>
" Last Change:  2011-7-27

setlocal iskeyword+=:

syn keyword dcmBuiltin dup clone r del repr swap clear empty?
syn keyword dcmBuiltin fold map len upcopy inherit obj me
syn keyword dcmBuiltin true false none
syn keyword dcmStatement push pop peek
syn keyword dcmRepeat while until foreach for
syn keyword dcmConditional if and or
syn keyword dcmException error catch try
syn keyword dcmInclude load
syn keyword dcmFunction x o def
syn keyword dcmType class : array def

syn match dcmComment /#.*/
syn match dcmNumber /\(^\|\s\|\[\)\d\+\(\.\d*\)\?/
syn match dcmNumber /\(^\|\s\|\[\)0x\x\+/
syn match dcmSymbol /\(^\|\s\|\[\),\S\+/
syn match dcmChar   /\(^\|\s\|\[\)'\([^\\]\|\\\(\D\|0x\x\+\|\d\+\)\)/
syn match dcmStack /\(^\|\s\|\[\)[$@~?]\S*/

syn region dcmString start=/"/ skip=/\\"/ end=/"/

hi link dcmStatement Statement
hi link dcmBuiltin Function
hi link dcmRepeat Repeat
hi link dcmConditional Conditional
hi link dcmException Exception
hi link dcmInclude Include
hi link dcmFunction Function
hi link dcmType Type

hi link dcmComment Comment
hi link dcmNumber Number
hi link dcmSymbol String
hi link dcmChar String
hi link dcmString String
hi link dcmStack Special

syn match dcmBrackets /[][]/
hi link dcmBrackets NONE
