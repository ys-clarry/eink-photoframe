document.addEventListener("DOMContentLoaded", () => {
  const uploadBtn = document.getElementById("uploadBtn");
  const sendBtn = document.getElementById("sendBtn");
  const fileInput = document.getElementById("fileInput");
  const statusText = document.getElementById("status");
  const canvas = document.getElementById("previewCanvas");
  const ctx = canvas.getContext("2d");

  let resizedBlob = null;

  uploadBtn.addEventListener("click", () => fileInput.click());

  fileInput.addEventListener("change", () => {
    const file = fileInput.files[0];
    if (!file) return;

    const img = new Image();
    img.onload = () => {
      // Resize image into canvas
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      ctx.drawImage(img, 0, 0, canvas.width, canvas.height);

      // Convert to grayscale here (optional visual)
      const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
      const data = imageData.data;
      for (let i = 0; i < data.length; i += 4) {
        const gray = data[i] * 0.3 + data[i + 1] * 0.59 + data[i + 2] * 0.11;
        data[i] = data[i + 1] = data[i + 2] = gray;
      }
      ctx.putImageData(imageData, 0, 0);

      // Export to JPEG blob
      canvas.toBlob(
        (blob) => {
          resizedBlob = blob;
          statusText.textContent = resizedBlob
            ? "Image loaded and resized."
            : "Failed to resize image.";
        },
        "image/jpeg",
        1.0
      );
    };
    img.src = URL.createObjectURL(file);
  });

  sendBtn.addEventListener("click", () => {
    if (!resizedBlob) {
      statusText.textContent = "Please upload and resize an image first.";
      return;
    }
    const formData = new FormData();
    formData.append("image", resizedBlob, "converted.jpg");
    fetch("/upload", {
      method: "POST",
      body: formData,
    }).then(() => (statusText.textContent = "Image sent to server."));
  });
});
