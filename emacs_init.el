;;; init.el --- Emacs startup configuration  -*- lexical-binding: t; -*-




;; Display tabs 8 columns wide by default.
(setq-default tab-width 8)

;; C/C++: indent one level = 4 columns, inserted as spaces.
(setq-default c-basic-offset 4)          ; classic cc-mode (c-mode, c++-mode, ...)
(setq-default c-ts-mode-indent-offset 4) ; tree-sitter modes (Emacs 29+)

(defun my-c-indent-setup ()
  "Use 4-space indentation in C-like buffers."
  (setq c-basic-offset 4
        tab-width 8
        indent-tabs-mode nil))           ; spaces, never literal TABs

(add-hook 'c-mode-common-hook #'my-c-indent-setup)
(add-hook 'c-ts-base-mode-hook #'my-c-indent-setup)

;; Load the Linux kernel C coding style.
;;(load (expand-file-name "linux-kernel-style.el" user-emacs-directory))

;;; init.el ends here
