test:
	@for test in hw0-backend_files/*; do \
		if [ -f "$$test" ]; then \
			echo "=================================="; \
			echo "Запуск $$(basename $$test)..."; \
			echo "=================================="; \
			./$$test; \
			echo ""; \
		fi \
	done