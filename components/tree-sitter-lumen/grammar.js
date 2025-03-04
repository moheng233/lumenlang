/**
 * @file Lumen grammar for tree-sitter
 * @author Moheng <ylm200322400@163.com>
 * @license MIT
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: "lumen",

  word: $ => $.identifier,

  extras: $ => [/\s|\\\r?\n/, $.comment],

  rules: {
    source_file: $ => repeat(choice($.method_define)),

    method_define: $ => seq("fun", $.identifier, "(", ")", optional(seq(":", $.identifier))),

    body: $ => seq("{", "}"),

    true: _ => token('true'),
    false: _ => token('false'),
    null: _ => 'null',
    identifier: _ =>
      /(\$|_|\\u[0-9A-Fa-f]{4}|\\U[0-9A-Fa-f]{8})(\$|\\u[0-9A-Fa-f]{4}|\\U[0-9A-Fa-f]{8})*/,

    comment: _ => token(choice(
      seq('//', /(\\+(.|\r?\n)|[^\\\n])*/),
      seq(
        '/*',
        /[^*]*\*+([^/*][^*]*\*+)*/,
        '/',
      ),
    )),
  }
});
